



#ifdef WINDOWS
   #include <winsock2.h>
   #include <ws2tcpip.h>
   #include <wspiapi.h>
#endif
#include <cstring>

#include "udtCommon.h"
#include "core.h"
#include "queue.h"

using namespace std;

CUnitQueue::CUnitQueue():
m_pQEntry(NULL),
m_pCurrQueue(NULL),
m_pLastQueue(NULL),
m_iSize(0),
m_iCount(0),
m_iMSS(),
m_iIPversion()
{
}

CUnitQueue::~CUnitQueue()
{
   CQEntry* p = m_pQEntry;

   while (p != NULL)
   {
      delete [] p->m_pUnit;
      delete [] p->m_pBuffer;

      CQEntry* q = p;
      if (p == m_pLastQueue)
         p = NULL;
      else
         p = p->m_pNext;
      delete q;
   }
}

int CUnitQueue::init(int size, int mss, int version)
{
   CQEntry* tempq = NULL;
   CUnit* tempu = NULL;
   char* tempb = NULL;

   try
   {
      tempq = new CQEntry;
      tempu = new CUnit [size];
      tempb = new char [size * mss];
   }
   catch (...)
   {
      delete tempq;
      delete [] tempu;
      delete [] tempb;

      return -1;
   }

   for (int i = 0; i < size; ++ i)
   {
      tempu[i].m_iFlag = 0;
      tempu[i].m_Packet.m_pcData = tempb + i * mss;
   }
   tempq->m_pUnit = tempu;
   tempq->m_pBuffer = tempb;
   tempq->m_iSize = size;

   m_pQEntry = m_pCurrQueue = m_pLastQueue = tempq;
   m_pQEntry->m_pNext = m_pQEntry;

   m_pAvailUnit = m_pCurrQueue->m_pUnit;

   m_iSize = size;
   m_iMSS = mss;
   m_iIPversion = version;

   return 0;
}

int CUnitQueue::increase()
{
   
   int real_count = 0;
   CQEntry* p = m_pQEntry;
   while (p != NULL)
   {
      CUnit* u = p->m_pUnit;
      for (CUnit* end = u + p->m_iSize; u != end; ++ u)
         if (u->m_iFlag != 0)
            ++ real_count;

      if (p == m_pLastQueue)
         p = NULL;
      else
         p = p->m_pNext;
   }
   m_iCount = real_count;
   if (double(m_iCount) / m_iSize < 0.9)
      return -1;

   CQEntry* tempq = NULL;
   CUnit* tempu = NULL;
   char* tempb = NULL;

   
   int size = m_pQEntry->m_iSize;

   try
   {
      tempq = new CQEntry;
      tempu = new CUnit [size];
      tempb = new char [size * m_iMSS];
   }
   catch (...)
   {
      delete tempq;
      delete [] tempu;
      delete [] tempb;

      return -1;
   }

   for (int i = 0; i < size; ++ i)
   {
      tempu[i].m_iFlag = 0;
      tempu[i].m_Packet.m_pcData = tempb + i * m_iMSS;
   }
   tempq->m_pUnit = tempu;
   tempq->m_pBuffer = tempb;
   tempq->m_iSize = size;

   m_pLastQueue->m_pNext = tempq;
   m_pLastQueue = tempq;
   m_pLastQueue->m_pNext = m_pQEntry;

   m_iSize += size;

   return 0;
}

int CUnitQueue::shrink()
{
   
   return -1;
}

CUnit* CUnitQueue::getNextAvailUnit()
{
   if (m_iCount * 10 > m_iSize * 9)
      increase();

   if (m_iCount >= m_iSize)
      return NULL;

   CQEntry* entrance = m_pCurrQueue;

   do
   {
      for (CUnit* sentinel = m_pCurrQueue->m_pUnit + m_pCurrQueue->m_iSize - 1; m_pAvailUnit != sentinel; ++ m_pAvailUnit)
         if (m_pAvailUnit->m_iFlag == 0)
            return m_pAvailUnit;

      if (m_pCurrQueue->m_pUnit->m_iFlag == 0)
      {
         m_pAvailUnit = m_pCurrQueue->m_pUnit;
         return m_pAvailUnit;
      }

      m_pCurrQueue = m_pCurrQueue->m_pNext;
      m_pAvailUnit = m_pCurrQueue->m_pUnit;
   } while (m_pCurrQueue != entrance);

   increase();

   return NULL;
}


CSndUList::CSndUList():
m_pHeap(NULL),
m_iArrayLength(4096),
m_iLastEntry(-1),
m_ListLock(),
m_pWindowLock(NULL),
m_pWindowCond(NULL),
m_pTimer(NULL)
{
   m_pHeap = new CSNode*[m_iArrayLength];

   #ifndef WINDOWS
      pthread_mutex_init(&m_ListLock, NULL);
   #else
      m_ListLock = CreateMutex(NULL, false, NULL);
   #endif
}

CSndUList::~CSndUList()
{
   delete [] m_pHeap;

   #ifndef WINDOWS
      pthread_mutex_destroy(&m_ListLock);
   #else
      CloseHandle(m_ListLock);
   #endif
}

void CSndUList::insert(int64_t ts, const CUDT* u)
{
   CGuard listguard(m_ListLock);

   
   if (m_iLastEntry == m_iArrayLength - 1)
   {
      CSNode** temp = NULL;

      try
      {
         temp = new CSNode*[m_iArrayLength * 2];
      }
      catch(...)
      {
         return;
      }

      memcpy(temp, m_pHeap, sizeof(CSNode*) * m_iArrayLength);
      m_iArrayLength *= 2;
      delete [] m_pHeap;
      m_pHeap = temp;
   }

   insert_(ts, u);
}

void CSndUList::update(const CUDT* u, bool reschedule)
{
   CGuard listguard(m_ListLock);

   CSNode* n = u->m_pSNode;

   if (n->m_iHeapLoc >= 0)
   {
      if (!reschedule)
         return;

      if (n->m_iHeapLoc == 0)
      {
         n->m_llTimeStamp = 1;
         m_pTimer->interrupt();
         return;
      }

      remove_(u);
   }

   insert_(1, u);
}

int CSndUList::pop(sockaddr*& addr, CPacket& pkt)
{
   CGuard listguard(m_ListLock);

   if (-1 == m_iLastEntry)
      return -1;

   
   uint64_t ts;
   CTimer::rdtsc(ts);
   if (ts < m_pHeap[0]->m_llTimeStamp)
      return -1;

   CUDT* u = m_pHeap[0]->m_pUDT;
   remove_(u);

   if (!u->m_bConnected || u->m_bBroken)
      return -1;

   
   if (u->packData(pkt, ts) <= 0)
      return -1;

   addr = u->m_pPeerAddr;

   
   if (ts > 0)
      insert_(ts, u);

   return 1;
}

void CSndUList::remove(const CUDT* u)
{
   CGuard listguard(m_ListLock);

   remove_(u);
}

uint64_t CSndUList::getNextProcTime()
{
   CGuard listguard(m_ListLock);

   if (-1 == m_iLastEntry)
      return 0;

   return m_pHeap[0]->m_llTimeStamp;
}

void CSndUList::insert_(int64_t ts, const CUDT* u)
{
   CSNode* n = u->m_pSNode;

   
   if (n->m_iHeapLoc >= 0)
      return;

   m_iLastEntry ++;
   m_pHeap[m_iLastEntry] = n;
   n->m_llTimeStamp = ts;

   int q = m_iLastEntry;
   int p = q;
   while (p != 0)
   {
      p = (q - 1) >> 1;
      if (m_pHeap[p]->m_llTimeStamp > m_pHeap[q]->m_llTimeStamp)
      {
         CSNode* t = m_pHeap[p];
         m_pHeap[p] = m_pHeap[q];
         m_pHeap[q] = t;
         t->m_iHeapLoc = q;
         q = p;
      }
      else
         break;
   }

   n->m_iHeapLoc = q;

   
   if (n->m_iHeapLoc == 0)
      m_pTimer->interrupt();

   
   if (0 == m_iLastEntry)
   {
      #ifndef WINDOWS
         pthread_mutex_lock(m_pWindowLock);
         pthread_cond_signal(m_pWindowCond);
         pthread_mutex_unlock(m_pWindowLock);
      #else
         SetEvent(*m_pWindowCond);
      #endif
   }
}

void CSndUList::remove_(const CUDT* u)
{
   CSNode* n = u->m_pSNode;

   if (n->m_iHeapLoc >= 0)
   {
      
      m_pHeap[n->m_iHeapLoc] = m_pHeap[m_iLastEntry];
      m_iLastEntry --;
      m_pHeap[n->m_iHeapLoc]->m_iHeapLoc = n->m_iHeapLoc;

      int q = n->m_iHeapLoc;
      int p = q * 2 + 1;
      while (p <= m_iLastEntry)
      {
         if ((p + 1 <= m_iLastEntry) && (m_pHeap[p]->m_llTimeStamp > m_pHeap[p + 1]->m_llTimeStamp))
            p ++;

         if (m_pHeap[q]->m_llTimeStamp > m_pHeap[p]->m_llTimeStamp)
         {
            CSNode* t = m_pHeap[p];
            m_pHeap[p] = m_pHeap[q];
            m_pHeap[p]->m_iHeapLoc = p;
            m_pHeap[q] = t;
            m_pHeap[q]->m_iHeapLoc = q;

            q = p;
            p = q * 2 + 1;
         }
         else
            break;
      }

      n->m_iHeapLoc = -1;
   }

   
   if (0 == m_iLastEntry)
      m_pTimer->interrupt();
}


CSndQueue::CSndQueue():
m_WorkerThread(),
m_pSndUList(NULL),
m_pChannel(NULL),
m_pTimer(NULL),
m_WindowLock(),
m_WindowCond(),
m_bClosing(false),
m_ExitCond()
{
   #ifndef WINDOWS
      pthread_cond_init(&m_WindowCond, NULL);
      pthread_mutex_init(&m_WindowLock, NULL);
   #else
      m_WindowLock = CreateMutex(NULL, false, NULL);
      m_WindowCond = CreateEvent(NULL, false, false, NULL);
      m_ExitCond = CreateEvent(NULL, false, false, NULL);
   #endif
}

CSndQueue::~CSndQueue()
{
   m_bClosing = true;

   #ifndef WINDOWS
      pthread_mutex_lock(&m_WindowLock);
      pthread_cond_signal(&m_WindowCond);
      pthread_mutex_unlock(&m_WindowLock);
      if (0 != m_WorkerThread)
         pthread_join(m_WorkerThread, NULL);
      pthread_cond_destroy(&m_WindowCond);
      pthread_mutex_destroy(&m_WindowLock);
   #else
      SetEvent(m_WindowCond);
      if (NULL != m_WorkerThread)
         WaitForSingleObject(m_ExitCond, INFINITE);
      CloseHandle(m_WorkerThread);
      CloseHandle(m_WindowLock);
      CloseHandle(m_WindowCond);
      CloseHandle(m_ExitCond);
   #endif

   delete m_pSndUList;
}

void CSndQueue::init(CChannel* c, CTimer* t)
{
   m_pChannel = c;
   m_pTimer = t;
   m_pSndUList = new CSndUList;
   m_pSndUList->m_pWindowLock = &m_WindowLock;
   m_pSndUList->m_pWindowCond = &m_WindowCond;
   m_pSndUList->m_pTimer = m_pTimer;

   #ifndef WINDOWS
      if (0 != pthread_create(&m_WorkerThread, NULL, CSndQueue::worker, this))
      {
         m_WorkerThread = 0;
         throw CUDTException(3, 1);
      }
   #else
      DWORD threadID;
      m_WorkerThread = CreateThread(NULL, 0, CSndQueue::worker, this, 0, &threadID);
      if (NULL == m_WorkerThread)
         throw CUDTException(3, 1);
   #endif
}

#ifndef WINDOWS
   void* CSndQueue::worker(void* param)
#else
   DWORD WINAPI CSndQueue::worker(LPVOID param)
#endif
{
   CSndQueue* self = (CSndQueue*)param;

   while (!self->m_bClosing)
   {
      uint64_t ts = self->m_pSndUList->getNextProcTime();

      if (ts > 0)
      {
         
         uint64_t currtime;
         CTimer::rdtsc(currtime);
         if (currtime < ts)
            self->m_pTimer->sleepto(ts);

         
         sockaddr* addr;
         CPacket pkt;
         if (self->m_pSndUList->pop(addr, pkt) < 0)
            continue;

         self->m_pChannel->sendto(addr, pkt);
      }
      else
      {
         
         #ifndef WINDOWS
            pthread_mutex_lock(&self->m_WindowLock);
            if (!self->m_bClosing && (self->m_pSndUList->m_iLastEntry < 0))
               pthread_cond_wait(&self->m_WindowCond, &self->m_WindowLock);
            pthread_mutex_unlock(&self->m_WindowLock);
         #else
            WaitForSingleObject(self->m_WindowCond, INFINITE);
         #endif
      }
   }

   #ifndef WINDOWS
      return NULL;
   #else
      SetEvent(self->m_ExitCond);
      return 0;
   #endif
}

int CSndQueue::sendto(const sockaddr* addr, CPacket& packet)
{
   
   m_pChannel->sendto(addr, packet);
   return packet.getLength();
}



CRcvUList::CRcvUList():
m_pUList(NULL),
m_pLast(NULL)
{
}

CRcvUList::~CRcvUList()
{
}

void CRcvUList::insert(const CUDT* u)
{
   CRNode* n = u->m_pRNode;
   CTimer::rdtsc(n->m_llTimeStamp);

   if (NULL == m_pUList)
   {
      
      n->m_pPrev = n->m_pNext = NULL;
      m_pLast = m_pUList = n;

      return;
   }

   
   n->m_pPrev = m_pLast;
   n->m_pNext = NULL;
   m_pLast->m_pNext = n;
   m_pLast = n;
}

void CRcvUList::remove(const CUDT* u)
{
   CRNode* n = u->m_pRNode;

   if (!n->m_bOnList)
      return;

   if (NULL == n->m_pPrev)
   {
      
      m_pUList = n->m_pNext;
      if (NULL == m_pUList)
         m_pLast = NULL;
      else
         m_pUList->m_pPrev = NULL;
   }
   else
   {
      n->m_pPrev->m_pNext = n->m_pNext;
      if (NULL == n->m_pNext)
      {
         
         m_pLast = n->m_pPrev;
      }
      else
         n->m_pNext->m_pPrev = n->m_pPrev;
   }

   n->m_pNext = n->m_pPrev = NULL;
}

void CRcvUList::update(const CUDT* u)
{
   CRNode* n = u->m_pRNode;

   if (!n->m_bOnList)
      return;

   CTimer::rdtsc(n->m_llTimeStamp);

   
   if (NULL == n->m_pNext)
      return;

   if (NULL == n->m_pPrev)
   {
      m_pUList = n->m_pNext;
      m_pUList->m_pPrev = NULL;
   }
   else
   {
      n->m_pPrev->m_pNext = n->m_pNext;
      n->m_pNext->m_pPrev = n->m_pPrev;
   }

   n->m_pPrev = m_pLast;
   n->m_pNext = NULL;
   m_pLast->m_pNext = n;
   m_pLast = n;
}


CHash::CHash():
m_pBucket(NULL),
m_iHashSize(0)
{
}

CHash::~CHash()
{
   for (int i = 0; i < m_iHashSize; ++ i)
   {
      CBucket* b = m_pBucket[i];
      while (NULL != b)
      {
         CBucket* n = b->m_pNext;
         delete b;
         b = n;
      }
   }

   delete [] m_pBucket;
}

void CHash::init(int size)
{
   m_pBucket = new CBucket* [size];

   for (int i = 0; i < size; ++ i)
      m_pBucket[i] = NULL;

   m_iHashSize = size;
}

CUDT* CHash::lookup(int32_t id)
{
   
   CBucket* b = m_pBucket[id % m_iHashSize];

   while (NULL != b)
   {
      if (id == b->m_iID)
         return b->m_pUDT;
      b = b->m_pNext;
   }

   return NULL;
}

void CHash::insert(int32_t id, CUDT* u)
{
   CBucket* b = m_pBucket[id % m_iHashSize];

   CBucket* n = new CBucket;
   n->m_iID = id;
   n->m_pUDT = u;
   n->m_pNext = b;

   m_pBucket[id % m_iHashSize] = n;
}

void CHash::remove(int32_t id)
{
   CBucket* b = m_pBucket[id % m_iHashSize];
   CBucket* p = NULL;

   while (NULL != b)
   {
      if (id == b->m_iID)
      {
         if (NULL == p)
            m_pBucket[id % m_iHashSize] = b->m_pNext;
         else
            p->m_pNext = b->m_pNext;

         delete b;

         return;
      }

      p = b;
      b = b->m_pNext;
   }
}



CRendezvousQueue::CRendezvousQueue():
m_lRendezvousID(),
m_RIDVectorLock()
{
   #ifndef WINDOWS
      pthread_mutex_init(&m_RIDVectorLock, NULL);
   #else
      m_RIDVectorLock = CreateMutex(NULL, false, NULL);
   #endif
}

CRendezvousQueue::~CRendezvousQueue()
{
   #ifndef WINDOWS
      pthread_mutex_destroy(&m_RIDVectorLock);
   #else
      CloseHandle(m_RIDVectorLock);
   #endif

   for (list<CRL>::iterator i = m_lRendezvousID.begin(); i != m_lRendezvousID.end(); ++ i)
   {
      if (AF_INET == i->m_iIPversion)
         delete (sockaddr_in*)i->m_pPeerAddr;
      else
         delete (sockaddr_in6*)i->m_pPeerAddr;
   }

   m_lRendezvousID.clear();
}

void CRendezvousQueue::insert(const UDTSOCKET& id, CUDT* u, int ipv, const sockaddr* addr, uint64_t ttl)
{
   CGuard vg(m_RIDVectorLock);

   CRL r;
   r.m_iID = id;
   r.m_pUDT = u;
   r.m_iIPversion = ipv;
   r.m_pPeerAddr = (AF_INET == ipv) ? (sockaddr*)new sockaddr_in : (sockaddr*)new sockaddr_in6;
   memcpy(r.m_pPeerAddr, addr, (AF_INET == ipv) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6));
   r.m_ullTTL = ttl;

   m_lRendezvousID.push_back(r);
}

void CRendezvousQueue::remove(const UDTSOCKET& id)
{
   CGuard vg(m_RIDVectorLock);

   for (list<CRL>::iterator i = m_lRendezvousID.begin(); i != m_lRendezvousID.end(); ++ i)
   {
      if (i->m_iID == id)
      {
         if (AF_INET == i->m_iIPversion)
            delete (sockaddr_in*)i->m_pPeerAddr;
         else
            delete (sockaddr_in6*)i->m_pPeerAddr;

         m_lRendezvousID.erase(i);

         return;
      }
   }
}

CUDT* CRendezvousQueue::retrieve(const sockaddr* addr, UDTSOCKET& id)
{
   CGuard vg(m_RIDVectorLock);

   
   for (list<CRL>::iterator i = m_lRendezvousID.begin(); i != m_lRendezvousID.end(); ++ i)
   {
      if (CIPAddress::ipcmp(addr, i->m_pPeerAddr, i->m_iIPversion) && ((0 == id) || (id == i->m_iID)))
      {
         id = i->m_iID;
         return i->m_pUDT;
      }
   }

   return NULL;
}

void CRendezvousQueue::updateConnStatus()
{
   if (m_lRendezvousID.empty())
      return;

   CGuard vg(m_RIDVectorLock);

   for (list<CRL>::iterator i = m_lRendezvousID.begin(); i != m_lRendezvousID.end(); ++ i)
   {
      
      if (CTimer::getTime() - i->m_pUDT->m_llLastReqTime > 250000)
      {
         if (CTimer::getTime() >= i->m_ullTTL)
         {
            
            i->m_pUDT->m_bConnecting = false;
            CUDT::s_UDTUnited.m_EPoll.update_events(i->m_iID, i->m_pUDT->m_sPollID, UDT_EPOLL_ERR, true);
            continue;
         }

         CPacket request;
         char* reqdata = new char [i->m_pUDT->m_iPayloadSize];
         request.pack(0, NULL, reqdata, i->m_pUDT->m_iPayloadSize);
         
         request.m_iID = !i->m_pUDT->m_bRendezvous ? 0 : i->m_pUDT->m_ConnRes.m_iID;
         int hs_size = i->m_pUDT->m_iPayloadSize;
         i->m_pUDT->m_ConnReq.serialize(reqdata, hs_size);
         request.setLength(hs_size);
         i->m_pUDT->m_pSndQueue->sendto(i->m_pPeerAddr, request);
         i->m_pUDT->m_llLastReqTime = CTimer::getTime();
         delete [] reqdata;
      }
   }
}


CRcvQueue::CRcvQueue():
m_WorkerThread(),
m_UnitQueue(),
m_pRcvUList(NULL),
m_pHash(NULL),
m_pChannel(NULL),
m_pTimer(NULL),
m_iPayloadSize(),
m_bClosing(false),
m_ExitCond(),
m_LSLock(),
m_pListener(NULL),
m_pRendezvousQueue(NULL),
m_vNewEntry(),
m_IDLock(),
m_mBuffer(),
m_PassLock(),
m_PassCond()
{
   #ifndef WINDOWS
      pthread_mutex_init(&m_PassLock, NULL);
      pthread_cond_init(&m_PassCond, NULL);
      pthread_mutex_init(&m_LSLock, NULL);
      pthread_mutex_init(&m_IDLock, NULL);
   #else
      m_PassLock = CreateMutex(NULL, false, NULL);
      m_PassCond = CreateEvent(NULL, false, false, NULL);
      m_LSLock = CreateMutex(NULL, false, NULL);
      m_IDLock = CreateMutex(NULL, false, NULL);
      m_ExitCond = CreateEvent(NULL, false, false, NULL);
   #endif
}

CRcvQueue::~CRcvQueue()
{
   m_bClosing = true;

   #ifndef WINDOWS
      if (0 != m_WorkerThread)
         pthread_join(m_WorkerThread, NULL);
      pthread_mutex_destroy(&m_PassLock);
      pthread_cond_destroy(&m_PassCond);
      pthread_mutex_destroy(&m_LSLock);
      pthread_mutex_destroy(&m_IDLock);
   #else
      if (NULL != m_WorkerThread)
         WaitForSingleObject(m_ExitCond, INFINITE);
      CloseHandle(m_WorkerThread);
      CloseHandle(m_PassLock);
      CloseHandle(m_PassCond);
      CloseHandle(m_LSLock);
      CloseHandle(m_IDLock);
      CloseHandle(m_ExitCond);
   #endif

   delete m_pRcvUList;
   delete m_pHash;
   delete m_pRendezvousQueue;

   
   for (map<int32_t, std::queue<CPacket*> >::iterator i = m_mBuffer.begin(); i != m_mBuffer.end(); ++ i)
   {
      while (!i->second.empty())
      {
         CPacket* pkt = i->second.front();
         delete [] pkt->m_pcData;
         delete pkt;
         i->second.pop();
      }
   }
}

void CRcvQueue::init(int qsize, int payload, int version, int hsize, CChannel* cc, CTimer* t)
{
   m_iPayloadSize = payload;

   m_UnitQueue.init(qsize, payload, version);

   m_pHash = new CHash;
   m_pHash->init(hsize);

   m_pChannel = cc;
   m_pTimer = t;

   m_pRcvUList = new CRcvUList;
   m_pRendezvousQueue = new CRendezvousQueue;

   #ifndef WINDOWS
      if (0 != pthread_create(&m_WorkerThread, NULL, CRcvQueue::worker, this))
      {
         m_WorkerThread = 0;
         throw CUDTException(3, 1);
      }
   #else
      DWORD threadID;
      m_WorkerThread = CreateThread(NULL, 0, CRcvQueue::worker, this, 0, &threadID);
      if (NULL == m_WorkerThread)
         throw CUDTException(3, 1);
   #endif
}

#ifndef WINDOWS
   void* CRcvQueue::worker(void* param)
#else
   DWORD WINAPI CRcvQueue::worker(LPVOID param)
#endif
{
   CRcvQueue* self = (CRcvQueue*)param;

   sockaddr* addr = (AF_INET == self->m_UnitQueue.m_iIPversion) ? (sockaddr*) new sockaddr_in : (sockaddr*) new sockaddr_in6;
   CUDT* u = NULL;
   int32_t id;

   while (!self->m_bClosing)
   {
      #ifdef NO_BUSY_WAITING
         self->m_pTimer->tick();
      #endif

      
      while (self->ifNewEntry())
      {
         CUDT* ne = self->getNewEntry();
         if (NULL != ne)
         {
            self->m_pRcvUList->insert(ne);
            self->m_pHash->insert(ne->m_SocketID, ne);
         }
      }

      
      CUnit* unit = self->m_UnitQueue.getNextAvailUnit();
      if (NULL == unit)
      {
         
         CPacket temp;
         temp.m_pcData = new char[self->m_iPayloadSize];
         temp.setLength(self->m_iPayloadSize);
         self->m_pChannel->recvfrom(addr, temp);
         delete [] temp.m_pcData;
         goto TIMER_CHECK;
      }

      unit->m_Packet.setLength(self->m_iPayloadSize);

      
      if (self->m_pChannel->recvfrom(addr, unit->m_Packet) < 0)
         goto TIMER_CHECK;

      id = unit->m_Packet.m_iID;

      
      if (0 == id)
      {
         if (NULL != self->m_pListener)
            self->m_pListener->listen(addr, unit->m_Packet);
         else if (NULL != (u = self->m_pRendezvousQueue->retrieve(addr, id)))
         {
            
            
            if (!u->m_bSynRecving)
               u->connect(unit->m_Packet);
            else
               self->storePkt(id, unit->m_Packet.clone());
         }
      }
      else if (id > 0)
      {
         if (NULL != (u = self->m_pHash->lookup(id)))
         {
            if (CIPAddress::ipcmp(addr, u->m_pPeerAddr, u->m_iIPversion))
            {
               if (u->m_bConnected && !u->m_bBroken && !u->m_bClosing)
               {
                  if (0 == unit->m_Packet.getFlag())
                     u->processData(unit);
                  else
                     u->processCtrl(unit->m_Packet);

                  u->checkTimers();
                  self->m_pRcvUList->update(u);
               }
            }
         }
         else if (NULL != (u = self->m_pRendezvousQueue->retrieve(addr, id)))
         {
            if (!u->m_bSynRecving)
               u->connect(unit->m_Packet);
            else
               self->storePkt(id, unit->m_Packet.clone());
         }
      }

TIMER_CHECK:
      

      uint64_t currtime;
      CTimer::rdtsc(currtime);

      CRNode* ul = self->m_pRcvUList->m_pUList;
      uint64_t ctime = currtime - 100000 * CTimer::getCPUFrequency();
      while ((NULL != ul) && (ul->m_llTimeStamp < ctime))
      {
         CUDT* udt = ul->m_pUDT;

         if (udt->m_bConnected && !udt->m_bBroken && !udt->m_bClosing)
         {
            udt->checkTimers();
            self->m_pRcvUList->update(udt);
         }
         else
         {
            
            self->m_pHash->remove(udt->m_SocketID);
            self->m_pRcvUList->remove(udt);
            udt->m_pRNode->m_bOnList = false;
         }

         ul = self->m_pRcvUList->m_pUList;
      }

      
      self->m_pRendezvousQueue->updateConnStatus();
   }

   if (AF_INET == self->m_UnitQueue.m_iIPversion)
      delete (sockaddr_in*)addr;
   else
      delete (sockaddr_in6*)addr;

   #ifndef WINDOWS
      return NULL;
   #else
      SetEvent(self->m_ExitCond);
      return 0;
   #endif
}

int CRcvQueue::recvfrom(int32_t id, CPacket& packet)
{
   CGuard bufferlock(m_PassLock);

   map<int32_t, std::queue<CPacket*> >::iterator i = m_mBuffer.find(id);

   if (i == m_mBuffer.end())
   {
      #ifndef WINDOWS
         uint64_t now = CTimer::getTime();
         timespec timeout;

         timeout.tv_sec = now / 1000000 + 1;
         timeout.tv_nsec = (now % 1000000) * 1000;

         pthread_cond_timedwait(&m_PassCond, &m_PassLock, &timeout);
      #else
         ReleaseMutex(m_PassLock);
         WaitForSingleObject(m_PassCond, 1000);
         WaitForSingleObject(m_PassLock, INFINITE);
      #endif

      i = m_mBuffer.find(id);
      if (i == m_mBuffer.end())
      {
         packet.setLength(-1);
         return -1;
      }
   }

   
   CPacket* newpkt = i->second.front();

   if (packet.getLength() < newpkt->getLength())
   {
      packet.setLength(-1);
      return -1;
   }

   
   memcpy(packet.m_nHeader, newpkt->m_nHeader, CPacket::m_iPktHdrSize);
   memcpy(packet.m_pcData, newpkt->m_pcData, newpkt->getLength());
   packet.setLength(newpkt->getLength());

   delete [] newpkt->m_pcData;
   delete newpkt;

   
   
   i->second.pop();
   if (i->second.empty())
      m_mBuffer.erase(i);

   return packet.getLength();
}

int CRcvQueue::setListener(CUDT* u)
{
   CGuard lslock(m_LSLock);

   if (NULL != m_pListener)
      return -1;

   m_pListener = u;
   return 0;
}

void CRcvQueue::removeListener(const CUDT* u)
{
   CGuard lslock(m_LSLock);

   if (u == m_pListener)
      m_pListener = NULL;
}

void CRcvQueue::registerConnector(const UDTSOCKET& id, CUDT* u, int ipv, const sockaddr* addr, uint64_t ttl)
{
   m_pRendezvousQueue->insert(id, u, ipv, addr, ttl);
}

void CRcvQueue::removeConnector(const UDTSOCKET& id)
{
   m_pRendezvousQueue->remove(id);

   CGuard bufferlock(m_PassLock);

   map<int32_t, std::queue<CPacket*> >::iterator i = m_mBuffer.find(id);
   if (i != m_mBuffer.end())
   {
      while (!i->second.empty())
      {
         delete [] i->second.front()->m_pcData;
         delete i->second.front();
         i->second.pop();
      }
      m_mBuffer.erase(i);
   }
}

void CRcvQueue::setNewEntry(CUDT* u)
{
   CGuard listguard(m_IDLock);
   m_vNewEntry.push_back(u);
}

bool CRcvQueue::ifNewEntry()
{
   return !(m_vNewEntry.empty());
}

CUDT* CRcvQueue::getNewEntry()
{
   CGuard listguard(m_IDLock);

   if (m_vNewEntry.empty())
      return NULL;

   CUDT* u = (CUDT*)*(m_vNewEntry.begin());
   m_vNewEntry.erase(m_vNewEntry.begin());

   return u;
}

void CRcvQueue::storePkt(int32_t id, CPacket* pkt)
{
   CGuard bufferlock(m_PassLock);

   map<int32_t, std::queue<CPacket*> >::iterator i = m_mBuffer.find(id);

   if (i == m_mBuffer.end())
   {
      m_mBuffer[id].push(pkt);

      #ifndef WINDOWS
         pthread_cond_signal(&m_PassCond);
      #else
         SetEvent(m_PassCond);
      #endif
   }
   else
   {
      
      if (i->second.size() > 16)
         return;

      i->second.push(pkt);
   }
}
