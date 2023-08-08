



#ifndef __UDT_API_H__
#define __UDT_API_H__


#include <map>
#include <vector>
#include "udt.h"
#include "packet.h"
#include "queue.h"
#include "cache.h"
#include "epoll.h"

class CUDT;

class CUDTSocket
{
public:
   CUDTSocket();
   ~CUDTSocket();

   UDTSTATUS m_Status;                       

   uint64_t m_TimeStamp;                     

   int m_iIPversion;                         
   sockaddr* m_pSelfAddr;                    
   sockaddr* m_pPeerAddr;                    

   UDTSOCKET m_SocketID;                     
   UDTSOCKET m_ListenSocket;                 

   UDTSOCKET m_PeerID;                       
   int32_t m_iISN;                           

   CUDT* m_pUDT;                             

   std::set<UDTSOCKET>* m_pQueuedSockets;    
   std::set<UDTSOCKET>* m_pAcceptSockets;    

   udt_pthread_cond_t m_AcceptCond;              
   udt_pthread_mutex_t m_AcceptLock;             

   unsigned int m_uiBackLog;                 

   int m_iMuxID;                             

   udt_pthread_mutex_t m_ControlLock;            

private:
   CUDTSocket(const CUDTSocket&);
   CUDTSocket& operator=(const CUDTSocket&);
};



class CUDTUnited
{
friend class CUDT;
friend class CRendezvousQueue;

public:
   CUDTUnited();
   ~CUDTUnited();

public:

      
      
      
      
      
      

   int startup();

      
      
      
      
      
      

   int cleanup();

      
      
      
      
      
      
      

   UDTSOCKET newSocket(int af, int type);

      
      
      
      
      
      
      
      

   int newConnection(const UDTSOCKET listen, const sockaddr* peer, CHandShake* hs);

      
      
      
      
      
      

   CUDT* lookup(const UDTSOCKET u);

      
      
      
      
      
      

   UDTSTATUS getStatus(const UDTSOCKET u);

      

   int bind(const UDTSOCKET u, const sockaddr* name, int namelen);
   int bind(const UDTSOCKET u, UDPSOCKET udpsock);
   int listen(const UDTSOCKET u, int backlog);
   UDTSOCKET accept(const UDTSOCKET listen, sockaddr* addr, int* addrlen);
   int connect(const UDTSOCKET u, const sockaddr* name, int namelen);
   int flush(const UDTSOCKET u);
   int close(const UDTSOCKET u);
   int getpeername(const UDTSOCKET u, sockaddr* name, int* namelen);
   int getsockname(const UDTSOCKET u, sockaddr* name, int* namelen);
   int select(ud_set* readfds, ud_set* writefds, ud_set* exceptfds, const timeval* timeout);
   int selectEx(const std::vector<UDTSOCKET>& fds, std::vector<UDTSOCKET>* readfds, std::vector<UDTSOCKET>* writefds, std::vector<UDTSOCKET>* exceptfds, int64_t msTimeOut);
   int epoll_create();
   int epoll_add_usock(const int eid, const UDTSOCKET u, const int* events = NULL);
   int epoll_add_ssock(const int eid, const SYSSOCKET s, const int* events = NULL);
   int epoll_remove_usock(const int eid, const UDTSOCKET u);
   int epoll_remove_ssock(const int eid, const SYSSOCKET s);
   int epoll_wait(const int eid, std::set<UDTSOCKET>* readfds, std::set<UDTSOCKET>* writefds, int64_t msTimeOut, std::set<SYSSOCKET>* lrfds = NULL, std::set<SYSSOCKET>* lwfds = NULL);
   int epoll_release(const int eid);

   
   int epoll_update_usock(const int eid, const UDTSOCKET u, const int* events = NULL);
   
   int epoll_verify_usock(const int eid, const UDTSOCKET u, int* events);

      
      
      
      
      
      

   void setError(CUDTException* e);

      
      
      
      
      
      

   CUDTException* getError();

private:


private:
   std::map<UDTSOCKET, CUDTSocket*> m_Sockets;       

   udt_pthread_mutex_t m_ControlLock;                

   udt_pthread_mutex_t m_IDLock;                     
   UDTSOCKET m_SocketID;                             

   std::map<int64_t, std::set<UDTSOCKET> > m_PeerRec;

private:
   udt_pthread_key_t m_TLSError;                     
   #ifndef WINDOWS
      static void TLSDestroy(void* e) {if (NULL != e) delete (CUDTException*)e;}
   #else
      std::map<DWORD, CUDTException*> m_mTLSRecord;
      void checkTLSValue();
      udt_pthread_mutex_t m_TLSLock;
   #endif

private:
   void connect_complete(const UDTSOCKET u);
   CUDTSocket* locate(const UDTSOCKET u);
   CUDTSocket* locate(const sockaddr* peer, const UDTSOCKET id, int32_t isn);
   void updateMux(CUDTSocket* s, const sockaddr* addr = NULL, const UDPSOCKET* = NULL);
   void updateMux(CUDTSocket* s, const CUDTSocket* ls);

private:
   std::map<int, CMultiplexer> m_mMultiplexer;		
   udt_pthread_mutex_t m_MultiplexerLock;

private:
   CCache<CInfoBlock>* m_pCache;			

private:
   volatile bool m_bClosing;
   udt_pthread_mutex_t m_GCStopLock;
   udt_pthread_cond_t m_GCStopCond;

   udt_pthread_mutex_t m_InitLock;
   int m_iInstanceCount;				
   bool m_bGCStatus;					

   udt_pthread_t m_GCThread;
   #ifndef WINDOWS
      static void* garbageCollect(void*);
   #else
      static DWORD WINAPI garbageCollect(LPVOID);
   #endif

   std::map<UDTSOCKET, CUDTSocket*> m_ClosedSockets;   

   void checkBrokenSockets();
   void removeSocket(const UDTSOCKET u);

private:
   CEPoll m_EPoll;                                     

private:
   CUDTUnited(const CUDTUnited&);
   CUDTUnited& operator=(const CUDTUnited&);
};

#endif
