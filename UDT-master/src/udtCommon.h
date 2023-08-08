



#ifndef __UDT_COMMON_H__
#define __UDT_COMMON_H__


#ifndef WINDOWS
   #include <sys/time.h>
   #include <sys/uio.h>
   #include <pthread.h>
#else
   #include <stdint.h>
   #include <ws2tcpip.h>
   #include <windows.h>
#endif
#include <cstdlib>
#include "udt.h"


#ifdef WINDOWS
    
    typedef HANDLE udt_pthread_t;
    typedef HANDLE udt_pthread_mutex_t;
    typedef HANDLE udt_pthread_cond_t;
    typedef DWORD udt_pthread_key_t;
#else
    typedef pthread_t udt_pthread_t;
    typedef pthread_mutex_t udt_pthread_mutex_t;
    typedef pthread_cond_t udt_pthread_cond_t;
    typedef pthread_key_t udt_pthread_key_t;
#endif




class CTimer
{
public:
   CTimer();
   ~CTimer();

public:

      
      
      
      
      
      

   void sleep(uint64_t interval);

      
      
      
      
      
      

   void sleepto(uint64_t nexttime);

      
      
      
      
      
      

   void interrupt();

      
      
      
      
      
      

   void tick();

public:

      
      
      
      
      
      

   static void rdtsc(uint64_t &x);

      
      
      
      
      
      

   static uint64_t getCPUFrequency();

      
      
      
      
      
      

   static uint64_t getTime();

      
      
      
      
      
      

   static void triggerEvent();

      
      
      
      
      
      

   static void waitForEvent();

      
      
      
      
      
      

   static void sleep();

private:
   uint64_t getTimeInMicroSec();

private:
   uint64_t m_ullSchedTime;             

   udt_pthread_cond_t m_TickCond;
   udt_pthread_mutex_t m_TickLock;

   static udt_pthread_cond_t m_EventCond;
   static udt_pthread_mutex_t m_EventLock;

private:
   static uint64_t s_ullCPUFrequency;	
   static uint64_t readCPUFrequency();
   static bool m_bUseMicroSecond;       
};



class CGuard
{
public:
   CGuard(udt_pthread_mutex_t& lock);
   ~CGuard();

public:
   static void enterCS(udt_pthread_mutex_t& lock);
   static void leaveCS(udt_pthread_mutex_t& lock);

   static void createMutex(udt_pthread_mutex_t& lock);
   static void releaseMutex(udt_pthread_mutex_t& lock);

   static void createCond(udt_pthread_cond_t& cond);
   static void releaseCond(udt_pthread_cond_t& cond);

private:
   udt_pthread_mutex_t& m_Mutex;        
   unsigned int m_iLocked;              

   CGuard& operator=(const CGuard&);
};














class CSeqNo
{
public:
   inline static int seqcmp(int32_t seq1, int32_t seq2)
   {return (abs(seq1 - seq2) < m_iSeqNoTH) ? (seq1 - seq2) : (seq2 - seq1);}

   inline static int seqlen(int32_t seq1, int32_t seq2)
   {return (seq1 <= seq2) ? (seq2 - seq1 + 1) : (seq2 - seq1 + m_iMaxSeqNo + 2);}

   inline static int seqoff(int32_t seq1, int32_t seq2)
   {
      if (abs(seq1 - seq2) < m_iSeqNoTH)
         return seq2 - seq1;

      if (seq1 < seq2)
         return seq2 - seq1 - m_iMaxSeqNo - 1;

      return seq2 - seq1 + m_iMaxSeqNo + 1;
   }

   inline static int32_t incseq(int32_t seq)
   {return (seq == m_iMaxSeqNo) ? 0 : seq + 1;}

   inline static int32_t decseq(int32_t seq)
   {return (seq == 0) ? m_iMaxSeqNo : seq - 1;}

   inline static int32_t incseq(int32_t seq, int32_t inc)
   {return (m_iMaxSeqNo - seq >= inc) ? seq + inc : seq - m_iMaxSeqNo + inc - 1;}

public:
   static const int32_t m_iSeqNoTH;             
   static const int32_t m_iMaxSeqNo;            
};





class CAckNo
{
public:
   inline static int32_t incack(int32_t ackno)
   {return (ackno == m_iMaxAckSeqNo) ? 0 : ackno + 1;}

public:
   static const int32_t m_iMaxAckSeqNo;         
};





class CMsgNo
{
public:
   inline static int msgcmp(int32_t msgno1, int32_t msgno2)
   {return (abs(msgno1 - msgno2) < m_iMsgNoTH) ? (msgno1 - msgno2) : (msgno2 - msgno1);}

   inline static int msglen(int32_t msgno1, int32_t msgno2)
   {return (msgno1 <= msgno2) ? (msgno2 - msgno1 + 1) : (msgno2 - msgno1 + m_iMaxMsgNo + 2);}

   inline static int msgoff(int32_t msgno1, int32_t msgno2)
   {
      if (abs(msgno1 - msgno2) < m_iMsgNoTH)
         return msgno2 - msgno1;

      if (msgno1 < msgno2)
         return msgno2 - msgno1 - m_iMaxMsgNo - 1;

      return msgno2 - msgno1 + m_iMaxMsgNo + 1;
   }

   inline static int32_t incmsg(int32_t msgno)
   {return (msgno == m_iMaxMsgNo) ? 0 : msgno + 1;}

public:
   static const int32_t m_iMsgNoTH;             
   static const int32_t m_iMaxMsgNo;            
};



struct CIPAddress
{
   static bool ipcmp(const sockaddr* addr1, const sockaddr* addr2, int ver = AF_INET);
   static void ntop(const sockaddr* addr, uint32_t ip[4], int ver = AF_INET);
   static void pton(sockaddr* addr, const uint32_t ip[4], int ver = AF_INET);
};



struct CMD5
{
   static void compute(const char* input, unsigned char result[16]);
};


#endif
