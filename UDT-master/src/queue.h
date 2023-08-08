




#ifndef __UDT_QUEUE_H__
#define __UDT_QUEUE_H__

#include "channel.h"
#include "packet.h"
#include <list>
#include <map>
#include <queue>
#include <vector>
#include "udtCommon.h"

class CUDT;

struct CUnit
{
   CPacket m_Packet;		
   int m_iFlag;			
};

class CUnitQueue
{
friend class CRcvQueue;
friend class CRcvBuffer;

public:
   CUnitQueue();
   ~CUnitQueue();

public:

      
      
      
      
      
      
      
      

   int init(int size, int mss, int version);

      
      
      
      
      
      

   int increase();

      
      
      
      
      
      

   int shrink();

      
      
      
      
      
      

   CUnit* getNextAvailUnit();

private:
   struct CQEntry
   {
      CUnit* m_pUnit;		
      char* m_pBuffer;		
      int m_iSize;		

      CQEntry* m_pNext;
   }
   *m_pQEntry,			
   *m_pCurrQueue,		
   *m_pLastQueue;		

   CUnit* m_pAvailUnit;         

   int m_iSize;			
   int m_iCount;		

   int m_iMSS;			
   int m_iIPversion;		

private:
   CUnitQueue(const CUnitQueue&);
   CUnitQueue& operator=(const CUnitQueue&);
};

struct CSNode
{
   CUDT* m_pUDT;		
   uint64_t m_llTimeStamp;      

   int m_iHeapLoc;		
};

class CSndUList
{
friend class CSndQueue;

public:
   CSndUList();
   ~CSndUList();

public:

      
      
      
      
      
      
      

   void insert(int64_t ts, const CUDT* u);

      
      
      
      
      
      
      

   void update(const CUDT* u, bool reschedule = true);

      
      
      
      
      
      
      

   int pop(sockaddr*& addr, CPacket& pkt);

      
      
      
      
      
      

   void remove(const CUDT* u);

      
      
      
      
      
      

   uint64_t getNextProcTime();

private:
   void insert_(int64_t ts, const CUDT* u);
   void remove_(const CUDT* u);

private:
   CSNode** m_pHeap;			
   int m_iArrayLength;			
   int m_iLastEntry;			

   udt_pthread_mutex_t m_ListLock;

   udt_pthread_mutex_t* m_pWindowLock;
   udt_pthread_cond_t* m_pWindowCond;

   CTimer* m_pTimer;

private:
   CSndUList(const CSndUList&);
   CSndUList& operator=(const CSndUList&);
};

struct CRNode
{
   CUDT* m_pUDT;                
   uint64_t m_llTimeStamp;      

   CRNode* m_pPrev;             
   CRNode* m_pNext;             

   bool m_bOnList;              
};

class CRcvUList
{
public:
   CRcvUList();
   ~CRcvUList();

public:

      
      
      
      
      
      

   void insert(const CUDT* u);

      
      
      
      
      
      

   void remove(const CUDT* u);

      
      
      
      
      
      

   void update(const CUDT* u);

public:
   CRNode* m_pUList;		

private:
   CRNode* m_pLast;		

private:
   CRcvUList(const CRcvUList&);
   CRcvUList& operator=(const CRcvUList&);
};

class CHash
{
public:
   CHash();
   ~CHash();

public:

      
      
      
      
      
      

   void init(int size);

      
      
      
      
      
      

   CUDT* lookup(int32_t id);

      
      
      
      
      
      
      

   void insert(int32_t id, CUDT* u);

      
      
      
      
      
      

   void remove(int32_t id);

private:
   struct CBucket
   {
      int32_t m_iID;		
      CUDT* m_pUDT;		

      CBucket* m_pNext;		
   } **m_pBucket;		

   int m_iHashSize;		

private:
   CHash(const CHash&);
   CHash& operator=(const CHash&);
};

class CRendezvousQueue
{
public:
   CRendezvousQueue();
   ~CRendezvousQueue();

public:
   void insert(const UDTSOCKET& id, CUDT* u, int ipv, const sockaddr* addr, uint64_t ttl);
   void remove(const UDTSOCKET& id);
   CUDT* retrieve(const sockaddr* addr, UDTSOCKET& id);

   void updateConnStatus();

private:
   struct CRL
   {
      UDTSOCKET m_iID;			
      CUDT* m_pUDT;			
      int m_iIPversion;                 
      sockaddr* m_pPeerAddr;		
      uint64_t m_ullTTL;			
   };
   std::list<CRL> m_lRendezvousID;      

   udt_pthread_mutex_t m_RIDVectorLock;
};

class CSndQueue
{
friend class CUDT;
friend class CUDTUnited;

public:
   CSndQueue();
   ~CSndQueue();

public:

      
      
      
      
      
      
      

   void init(CChannel* c, CTimer* t);

      
      
      
      
      
      
      

   int sendto(const sockaddr* addr, CPacket& packet);

private:
#ifndef WINDOWS
   static void* worker(void* param);
#else
   static DWORD WINAPI worker(LPVOID param);
#endif

   udt_pthread_t m_WorkerThread;

private:
   CSndUList* m_pSndUList;		
   CChannel* m_pChannel;                
   CTimer* m_pTimer;			

   udt_pthread_mutex_t m_WindowLock;
   udt_pthread_cond_t m_WindowCond;

   volatile bool m_bClosing;		
   udt_pthread_cond_t m_ExitCond;

private:
   CSndQueue(const CSndQueue&);
   CSndQueue& operator=(const CSndQueue&);
};

class CRcvQueue
{
friend class CUDT;
friend class CUDTUnited;

public:
   CRcvQueue();
   ~CRcvQueue();

public:

      
      
      
      
      
      
      
      
      
      
      

   void init(int size, int payload, int version, int hsize, CChannel* c, CTimer* t);

      
      
      
      
      
      
      

   int recvfrom(int32_t id, CPacket& packet);

private:
#ifndef WINDOWS
   static void* worker(void* param);
#else
   static DWORD WINAPI worker(LPVOID param);
#endif

   udt_pthread_t m_WorkerThread;

private:
   CUnitQueue m_UnitQueue;		

   CRcvUList* m_pRcvUList;		
   CHash* m_pHash;			
   CChannel* m_pChannel;		
   CTimer* m_pTimer;			

   int m_iPayloadSize;                  

   volatile bool m_bClosing;            
   udt_pthread_cond_t m_ExitCond;

private:
   int setListener(CUDT* u);
   void removeListener(const CUDT* u);

   void registerConnector(const UDTSOCKET& id, CUDT* u, int ipv, const sockaddr* addr, uint64_t ttl);
   void removeConnector(const UDTSOCKET& id);

   void setNewEntry(CUDT* u);
   bool ifNewEntry();
   CUDT* getNewEntry();

   void storePkt(int32_t id, CPacket* pkt);

private:
   udt_pthread_mutex_t m_LSLock;
   CUDT* m_pListener;                                   
   CRendezvousQueue* m_pRendezvousQueue;                

   std::vector<CUDT*> m_vNewEntry;                      
   udt_pthread_mutex_t m_IDLock;

   std::map<int32_t, std::queue<CPacket*> > m_mBuffer;	
   udt_pthread_mutex_t m_PassLock;
   udt_pthread_cond_t m_PassCond;

private:
   CRcvQueue(const CRcvQueue&);
   CRcvQueue& operator=(const CRcvQueue&);
};

struct CMultiplexer
{
   CSndQueue* m_pSndQueue;	
   CRcvQueue* m_pRcvQueue;	
   CChannel* m_pChannel;	
   CTimer* m_pTimer;		

   int m_iPort;			
   int m_iIPversion;		
   int m_iMSS;			
   int m_iRefCount;		
   bool m_bReusable;		

   int m_iID;			
};

#endif
