




#ifndef WINDOWS
   #include <cstring>
   #include <cerrno>
   #include <unistd.h>
   #include <stdio.h>
   #ifdef MACOSX
      #include <mach/mach_time.h>
   #endif
#else
   #include <winsock2.h>
   #include <ws2tcpip.h>
   #include <wspiapi.h>
#endif

#include <cmath>
#include "md5.h"
#include "udtCommon.h"

bool CTimer::m_bUseMicroSecond = false;
uint64_t CTimer::s_ullCPUFrequency = CTimer::readCPUFrequency();
#ifndef WINDOWS
   udt_pthread_mutex_t CTimer::m_EventLock = PTHREAD_MUTEX_INITIALIZER;
   udt_pthread_cond_t CTimer::m_EventCond = PTHREAD_COND_INITIALIZER;
#else
   udt_pthread_mutex_t CTimer::m_EventLock = CreateMutex(NULL, false, NULL);
   udt_pthread_cond_t CTimer::m_EventCond = CreateEvent(NULL, false, false, NULL);
#endif

CTimer::CTimer():
m_ullSchedTime(),
m_TickCond(),
m_TickLock()
{
   #ifndef WINDOWS
      pthread_mutex_init(&m_TickLock, NULL);
      pthread_cond_init(&m_TickCond, NULL);
   #else
      m_TickLock = CreateMutex(NULL, false, NULL);
      m_TickCond = CreateEvent(NULL, false, false, NULL);
   #endif
}

CTimer::~CTimer()
{
   #ifndef WINDOWS
      pthread_mutex_destroy(&m_TickLock);
      pthread_cond_destroy(&m_TickCond);
   #else
      CloseHandle(m_TickLock);
      CloseHandle(m_TickCond);
   #endif
}

void CTimer::rdtsc(uint64_t &x)
{
   if (m_bUseMicroSecond)
   {
      x = getTime();
      return;
   }

   #if defined(LINUX) && defined(I386)
      uint32_t lval, hval;
      
      
      asm volatile ("rdtsc" : "=a" (lval), "=d" (hval));
      
      x = hval;
      x = (x << 32) | lval;
   #elif defined(LINUX) && defined(AMD64)
      uint32_t lval, hval;
      asm ("rdtsc" : "=a" (lval), "=d" (hval));
      x = hval;
      x = (x << 32) | lval;
   #elif defined(WINDOWS)
      
      
      BOOL ret = QueryPerformanceCounter((LARGE_INTEGER *)&x);
      
      if (!ret)
         x = getTime() * s_ullCPUFrequency;
   #elif defined(MACOSX)
      x = mach_absolute_time();
   #else
      
      x = getTime() * s_ullCPUFrequency;
   #endif
}

uint64_t CTimer::readCPUFrequency()
{
   uint64_t frequency = 1;  

   #if defined(LINUX)
      
      float mhz = 0;
      char str[256] = {};
      char *p = NULL;
      int find = 0;
      FILE * fd = fopen("/proc/cpuinfo", "r");

      if (fd == NULL) {
         
      } else {
         while (fgets(str, 256, fd)) {
            if (strncmp("cpu MHz", str, sizeof("cpu MHz")-1) == 0) {
               find = 1;
               break;
            }
         }

         fclose(fd);
      }

      if (find) {
         int i=0;
         while(str[i++] != ':');

         p = str;
         p+=i;

         sscanf(p, "%f", &mhz);
         frequency = (uint64_t)mhz;
         
      } else {
         
         
         uint64_t t1, t2;

         rdtsc(t1);
         timespec ts;
         ts.tv_sec = 0;
         ts.tv_nsec = 100000000;
         nanosleep(&ts, NULL);
         rdtsc(t2);

         
         frequency = (t2 - t1) / 100000;
      }
   #elif defined(WINDOWS)
      int64_t ccf;
      if (QueryPerformanceFrequency((LARGE_INTEGER *)&ccf))
         frequency = ccf / 1000000;
   #elif defined(MACOSX)
      mach_timebase_info_data_t info;
      mach_timebase_info(&info);
      frequency = info.denom * 1000ULL / info.numer;
   #endif

   
   if (frequency < 10)
   {
      frequency = 1;
      m_bUseMicroSecond = true;
   }
   return frequency;
}

uint64_t CTimer::getCPUFrequency()
{
   return s_ullCPUFrequency;
}

void CTimer::sleep(uint64_t interval)
{
   uint64_t t;
   rdtsc(t);

   
   sleepto(t + interval);
}

void CTimer::sleepto(uint64_t nexttime)
{
   
   m_ullSchedTime = nexttime;

   uint64_t t;
   rdtsc(t);

   while (t < m_ullSchedTime)
   {
      #ifndef NO_BUSY_WAITING
         #if defined(LINUX) && defined(I386)
            __asm__ volatile ("pause; rep; nop; nop; nop; nop; nop;");
         #elif defined(LINUX) && defined(AMD64)
            __asm__ volatile ("nop; nop; nop; nop; nop;");
         #endif
      #else
         #ifndef WINDOWS
            timeval now;
            timespec timeout;
            gettimeofday(&now, 0);
            if (now.tv_usec < 990000)
            {
               timeout.tv_sec = now.tv_sec;
               timeout.tv_nsec = (now.tv_usec + 10000) * 1000;
            }
            else
            {
               timeout.tv_sec = now.tv_sec + 1;
               timeout.tv_nsec = (now.tv_usec + 10000 - 1000000) * 1000;
            }
            pthread_mutex_lock(&m_TickLock);
            pthread_cond_timedwait(&m_TickCond, &m_TickLock, &timeout);
            pthread_mutex_unlock(&m_TickLock);
         #else
            WaitForSingleObject(m_TickCond, 1);
         #endif
      #endif

      rdtsc(t);
   }
}

void CTimer::interrupt()
{
   
   rdtsc(m_ullSchedTime);
   tick();
}

void CTimer::tick()
{
   #ifndef WINDOWS
      pthread_cond_signal(&m_TickCond);
   #else
      SetEvent(m_TickCond);
   #endif
}

uint64_t CTimer::getTime()
{
   
   
   
   
   

   #ifndef WINDOWS
      timeval t;
      gettimeofday(&t, 0);
      return t.tv_sec * 1000000ULL + t.tv_usec;
   #else
      LARGE_INTEGER ccf;
      HANDLE hCurThread = ::GetCurrentThread();
      DWORD_PTR dwOldMask = ::SetThreadAffinityMask(hCurThread, 1);
      if (QueryPerformanceFrequency(&ccf))
      {
         LARGE_INTEGER cc;
         if (QueryPerformanceCounter(&cc))
         {
            SetThreadAffinityMask(hCurThread, dwOldMask);
            return (cc.QuadPart * 1000000ULL / ccf.QuadPart);
         }
      }

      SetThreadAffinityMask(hCurThread, dwOldMask);
      return GetTickCount() * 1000ULL;
   #endif
}

void CTimer::triggerEvent()
{
   #ifndef WINDOWS
      pthread_cond_signal(&m_EventCond);
   #else
      SetEvent(m_EventCond);
   #endif
}

void CTimer::waitForEvent()
{
   #ifndef WINDOWS
      timeval now;
      timespec timeout;
      gettimeofday(&now, 0);
      if (now.tv_usec < 990000)
      {
         timeout.tv_sec = now.tv_sec;
         timeout.tv_nsec = (now.tv_usec + 10000) * 1000;
      }
      else
      {
         timeout.tv_sec = now.tv_sec + 1;
         timeout.tv_nsec = (now.tv_usec + 10000 - 1000000) * 1000;
      }
      pthread_mutex_lock(&m_EventLock);
      pthread_cond_timedwait(&m_EventCond, &m_EventLock, &timeout);
      pthread_mutex_unlock(&m_EventLock);
   #else
      WaitForSingleObject(m_EventCond, 1);
   #endif
}

void CTimer::sleep()
{
   #ifndef WINDOWS
      usleep(10);
   #else
      Sleep(1);
   #endif
}




CGuard::CGuard(udt_pthread_mutex_t& lock):
m_Mutex(lock),
m_iLocked()
{
   #ifndef WINDOWS
      m_iLocked = pthread_mutex_lock(&m_Mutex);
   #else
      m_iLocked = WaitForSingleObject(m_Mutex, INFINITE);
   #endif
}


CGuard::~CGuard()
{
   #ifndef WINDOWS
      if (0 == m_iLocked)
         pthread_mutex_unlock(&m_Mutex);
   #else
      if (WAIT_FAILED != m_iLocked)
         ReleaseMutex(m_Mutex);
   #endif
}

void CGuard::enterCS(udt_pthread_mutex_t& lock)
{
   #ifndef WINDOWS
      pthread_mutex_lock(&lock);
   #else
      WaitForSingleObject(lock, INFINITE);
   #endif
}

void CGuard::leaveCS(udt_pthread_mutex_t& lock)
{
   #ifndef WINDOWS
      pthread_mutex_unlock(&lock);
   #else
      ReleaseMutex(lock);
   #endif
}

void CGuard::createMutex(udt_pthread_mutex_t& lock)
{
   #ifndef WINDOWS
      pthread_mutex_init(&lock, NULL);
   #else
      lock = CreateMutex(NULL, false, NULL);
   #endif
}

void CGuard::releaseMutex(udt_pthread_mutex_t& lock)
{
   #ifndef WINDOWS
      pthread_mutex_destroy(&lock);
   #else
      CloseHandle(lock);
   #endif
}

void CGuard::createCond(udt_pthread_cond_t& cond)
{
   #ifndef WINDOWS
      pthread_cond_init(&cond, NULL);
   #else
      cond = CreateEvent(NULL, false, false, NULL);
   #endif
}

void CGuard::releaseCond(udt_pthread_cond_t& cond)
{
   #ifndef WINDOWS
      pthread_cond_destroy(&cond);
   #else
      CloseHandle(cond);
   #endif

}


CUDTException::CUDTException(int major, int minor, int err):
m_iMajor(major),
m_iMinor(minor)
{
   if (-1 == err)
      #ifndef WINDOWS
         m_iErrno = errno;
      #else
         m_iErrno = GetLastError();
      #endif
   else
      m_iErrno = err;
}

CUDTException::CUDTException(const CUDTException& e):
m_iMajor(e.m_iMajor),
m_iMinor(e.m_iMinor),
m_iErrno(e.m_iErrno),
m_strMsg()
{
}

CUDTException::~CUDTException()
{
}

const char* CUDTException::getErrorMessage()
{
   

   switch (m_iMajor)
   {
      case 0:
        m_strMsg = "Success";
        break;

      case 1:
        m_strMsg = "Connection setup failure";

        switch (m_iMinor)
        {
        case 1:
           m_strMsg += ": connection time out";
           break;

        case 2:
           m_strMsg += ": connection rejected";
           break;

        case 3:
           m_strMsg += ": unable to create/configure UDP socket";
           break;

        case 4:
           m_strMsg += ": abort for security reasons";
           break;

        default:
           break;
        }

        break;

      case 2:
        switch (m_iMinor)
        {
        case 1:
           m_strMsg = "Connection was broken";
           break;

        case 2:
           m_strMsg = "Connection does not exist";
           break;

        default:
           break;
        }

        break;

      case 3:
        m_strMsg = "System resource failure";

        switch (m_iMinor)
        {
        case 1:
           m_strMsg += ": unable to create new threads";
           break;

        case 2:
           m_strMsg += ": unable to allocate buffers";
           break;

        default:
           break;
        }

        break;

      case 4:
        m_strMsg = "File system failure";

        switch (m_iMinor)
        {
        case 1:
           m_strMsg += ": cannot seek read position";
           break;

        case 2:
           m_strMsg += ": failure in read";
           break;

        case 3:
           m_strMsg += ": cannot seek write position";
           break;

        case 4:
           m_strMsg += ": failure in write";
           break;

        default:
           break;
        }

        break;

      case 5:
        m_strMsg = "Operation not supported";

        switch (m_iMinor)
        {
        case 1:
           m_strMsg += ": Cannot do this operation on a BOUND socket";
           break;

        case 2:
           m_strMsg += ": Cannot do this operation on a CONNECTED socket";
           break;

        case 3:
           m_strMsg += ": Bad parameters";
           break;

        case 4:
           m_strMsg += ": Invalid socket ID";
           break;

        case 5:
           m_strMsg += ": Cannot do this operation on an UNBOUND socket";
           break;

        case 6:
           m_strMsg += ": Socket is not in listening state";
           break;

        case 7:
           m_strMsg += ": Listen/accept is not supported in rendezous connection setup";
           break;

        case 8:
           m_strMsg += ": Cannot call connect on UNBOUND socket in rendezvous connection setup";
           break;

        case 9:
           m_strMsg += ": This operation is not supported in SOCK_STREAM mode";
           break;

        case 10:
           m_strMsg += ": This operation is not supported in SOCK_DGRAM mode";
           break;

        case 11:
           m_strMsg += ": Another socket is already listening on the same port";
           break;

        case 12:
           m_strMsg += ": Message is too large to send (it must be less than the UDT send buffer size)";
           break;

        case 13:
           m_strMsg += ": Invalid epoll ID";
           break;

        default:
           break;
        }

        break;

     case 6:
        m_strMsg = "Non-blocking call failure";

        switch (m_iMinor)
        {
        case 1:
           m_strMsg += ": no buffer available for sending";
           break;

        case 2:
           m_strMsg += ": no data available for reading";
           break;

        default:
           break;
        }

        break;

     case 7:
        m_strMsg = "The peer side has signalled an error";

        break;

      default:
        m_strMsg = "Unknown error";
   }

   
   if ((0 != m_iMajor) && (0 < m_iErrno))
   {
      m_strMsg += ": ";
      #ifndef WINDOWS
         char errmsg[1024];
         if (strerror_r(m_iErrno, errmsg, 1024) == 0)
            m_strMsg += errmsg;
      #else
         LPVOID lpMsgBuf;
         FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, m_iErrno, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
         m_strMsg += (char*)lpMsgBuf;
         LocalFree(lpMsgBuf);
      #endif
   }

   
   #ifndef WINDOWS
      m_strMsg += ".";
   #endif

   return m_strMsg.c_str();
}

int CUDTException::getErrorCode() const
{
   return m_iMajor * 1000 + m_iMinor;
}

void CUDTException::clear()
{
   m_iMajor = 0;
   m_iMinor = 0;
   m_iErrno = 0;
}

const int CUDTException::SUCCESS = 0;
const int CUDTException::ECONNSETUP = 1000;
const int CUDTException::ENOSERVER = 1001;
const int CUDTException::ECONNREJ = 1002;
const int CUDTException::ESOCKFAIL = 1003;
const int CUDTException::ESECFAIL = 1004;
const int CUDTException::ECONNFAIL = 2000;
const int CUDTException::ECONNLOST = 2001;
const int CUDTException::ENOCONN = 2002;
const int CUDTException::ERESOURCE = 3000;
const int CUDTException::ETHREAD = 3001;
const int CUDTException::ENOBUF = 3002;
const int CUDTException::EFILE = 4000;
const int CUDTException::EINVRDOFF = 4001;
const int CUDTException::ERDPERM = 4002;
const int CUDTException::EINVWROFF = 4003;
const int CUDTException::EWRPERM = 4004;
const int CUDTException::EINVOP = 5000;
const int CUDTException::EBOUNDSOCK = 5001;
const int CUDTException::ECONNSOCK = 5002;
const int CUDTException::EINVPARAM = 5003;
const int CUDTException::EINVSOCK = 5004;
const int CUDTException::EUNBOUNDSOCK = 5005;
const int CUDTException::ENOLISTEN = 5006;
const int CUDTException::ERDVNOSERV = 5007;
const int CUDTException::ERDVUNBOUND = 5008;
const int CUDTException::ESTREAMILL = 5009;
const int CUDTException::EDGRAMILL = 5010;
const int CUDTException::EDUPLISTEN = 5011;
const int CUDTException::ELARGEMSG = 5012;
const int CUDTException::EINVPOLLID = 5013;
const int CUDTException::EASYNCFAIL = 6000;
const int CUDTException::EASYNCSND = 6001;
const int CUDTException::EASYNCRCV = 6002;
const int CUDTException::ETIMEOUT = 6003;
const int CUDTException::EPEERERR = 7000;
const int CUDTException::EUNKNOWN = -1;



bool CIPAddress::ipcmp(const sockaddr* addr1, const sockaddr* addr2, int ver)
{
   if (AF_INET == ver)
   {
      sockaddr_in* a1 = (sockaddr_in*)addr1;
      sockaddr_in* a2 = (sockaddr_in*)addr2;

      if ((a1->sin_port == a2->sin_port) && (a1->sin_addr.s_addr == a2->sin_addr.s_addr))
         return true;
   }
   else
   {
      sockaddr_in6* a1 = (sockaddr_in6*)addr1;
      sockaddr_in6* a2 = (sockaddr_in6*)addr2;

      if (a1->sin6_port == a2->sin6_port)
      {
         for (int i = 0; i < 16; ++ i)
            if (*((char*)&(a1->sin6_addr) + i) != *((char*)&(a2->sin6_addr) + i))
               return false;

         return true;
      }
   }

   return false;
}

void CIPAddress::ntop(const sockaddr* addr, uint32_t ip[4], int ver)
{
   if (AF_INET == ver)
   {
      sockaddr_in* a = (sockaddr_in*)addr;
      ip[0] = a->sin_addr.s_addr;
   }
   else
   {
      sockaddr_in6* a = (sockaddr_in6*)addr;
      ip[3] = (a->sin6_addr.s6_addr[15] << 24) + (a->sin6_addr.s6_addr[14] << 16) + (a->sin6_addr.s6_addr[13] << 8) + a->sin6_addr.s6_addr[12];
      ip[2] = (a->sin6_addr.s6_addr[11] << 24) + (a->sin6_addr.s6_addr[10] << 16) + (a->sin6_addr.s6_addr[9] << 8) + a->sin6_addr.s6_addr[8];
      ip[1] = (a->sin6_addr.s6_addr[7] << 24) + (a->sin6_addr.s6_addr[6] << 16) + (a->sin6_addr.s6_addr[5] << 8) + a->sin6_addr.s6_addr[4];
      ip[0] = (a->sin6_addr.s6_addr[3] << 24) + (a->sin6_addr.s6_addr[2] << 16) + (a->sin6_addr.s6_addr[1] << 8) + a->sin6_addr.s6_addr[0];
   }
}

void CIPAddress::pton(sockaddr* addr, const uint32_t ip[4], int ver)
{
   if (AF_INET == ver)
   {
      sockaddr_in* a = (sockaddr_in*)addr;
      a->sin_addr.s_addr = ip[0];
   }
   else
   {
      sockaddr_in6* a = (sockaddr_in6*)addr;
      for (int i = 0; i < 4; ++ i)
      {
         a->sin6_addr.s6_addr[i * 4] = ip[i] & 0xFF;
         a->sin6_addr.s6_addr[i * 4 + 1] = (unsigned char)((ip[i] & 0xFF00) >> 8);
         a->sin6_addr.s6_addr[i * 4 + 2] = (unsigned char)((ip[i] & 0xFF0000) >> 16);
         a->sin6_addr.s6_addr[i * 4 + 3] = (unsigned char)((ip[i] & 0xFF000000) >> 24);
      }
   }
}


void CMD5::compute(const char* input, unsigned char result[16])
{
   md5_state_t state;

   md5_init(&state);
   md5_append(&state, (const md5_byte_t *)input, strlen(input));
   md5_finish(&state, result);
}
