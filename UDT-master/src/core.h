



#ifndef __UDT_CORE_H__
#define __UDT_CORE_H__


#include "udt.h"
#include "udtCommon.h"
#include "list.h"
#include "buffer.h"
#include "window.h"
#include "packet.h"
#include "channel.h"
#include "api.h"
#include "ccc.h"
#include "cache.h"
#include "queue.h"

enum UDTSockType {UDT_STREAM = 1, UDT_DGRAM};

class CUDT
{
friend class CUDTSocket;
friend class CUDTUnited;
friend class CCC;
friend struct CUDTComp;
friend class CCache<CInfoBlock>;
friend class CRendezvousQueue;
friend class CSndQueue;
friend class CRcvQueue;
friend class CSndUList;
friend class CRcvUList;

private: 
   CUDT();
   CUDT(const CUDT& ancestor);
   const CUDT& operator=(const CUDT&) {return *this;}
   ~CUDT();

public: 
   static int startup();
   static int cleanup();
   static UDTSOCKET socket(int af, int type = SOCK_STREAM, int protocol = 0);
   static int bind(UDTSOCKET u, const sockaddr* name, int namelen);
   static int bind(UDTSOCKET u, UDPSOCKET udpsock);
   static int listen(UDTSOCKET u, int backlog);
   static UDTSOCKET accept(UDTSOCKET u, sockaddr* addr, int* addrlen);
   static int connect(UDTSOCKET u, const sockaddr* name, int namelen);
   static int flush(UDTSOCKET u);
   static int close(UDTSOCKET u);
   static int getpeername(UDTSOCKET u, sockaddr* name, int* namelen);
   static int getsockname(UDTSOCKET u, sockaddr* name, int* namelen);
   static int getsockopt(UDTSOCKET u, int level, UDTOpt optname, void* optval, int* optlen);
   static int setsockopt(UDTSOCKET u, int level, UDTOpt optname, const void* optval, int optlen);
   static int send(UDTSOCKET u, const char* buf, int len, int flags);
   static int recv(UDTSOCKET u, char* buf, int len, int flags);
   static int sendmsg(UDTSOCKET u, const char* buf, int len, int ttl = -1, bool inorder = false);
   static int recvmsg(UDTSOCKET u, char* buf, int len);
   static int64_t sendfile(UDTSOCKET u, std::fstream& ifs, int64_t& offset, int64_t size, int block = 364000);
   static int64_t recvfile(UDTSOCKET u, std::fstream& ofs, int64_t& offset, int64_t size, int block = 7280000);
   static int select(int nfds, ud_set* readfds, ud_set* writefds, ud_set* exceptfds, const timeval* timeout);
   static int selectEx(const std::vector<UDTSOCKET>& fds, std::vector<UDTSOCKET>* readfds, std::vector<UDTSOCKET>* writefds, std::vector<UDTSOCKET>* exceptfds, int64_t msTimeOut);
   static int epoll_create();
   static int epoll_add_usock(const int eid, const UDTSOCKET u, const int* events = NULL);
   static int epoll_add_ssock(const int eid, const SYSSOCKET s, const int* events = NULL);
   static int epoll_remove_usock(const int eid, const UDTSOCKET u);
   static int epoll_remove_ssock(const int eid, const SYSSOCKET s);
   static int epoll_wait(const int eid, std::set<UDTSOCKET>* readfds, std::set<UDTSOCKET>* writefds, int64_t msTimeOut, std::set<SYSSOCKET>* lrfds = NULL, std::set<SYSSOCKET>* wrfds = NULL);
   static int epoll_release(const int eid);
   static CUDTException& getlasterror();
   static int perfmon(UDTSOCKET u, CPerfMon* perf, bool clear = true);
   static UDTSTATUS getsockstate(UDTSOCKET u);

   
   static int epoll_update_usock(const int eid, const UDTSOCKET u, const int* events = NULL);
   
   static int epoll_verify_usock(const int eid, const UDTSOCKET u, int* events);

public: 
   static CUDT* getUDTHandle(UDTSOCKET u);

private:
      
      
      
      
      
      

   void open();

      
      
      
      
      
      

   void listen();

      
      
      
      
      
      

   void connect(const sockaddr* peer);

      
      
      
      
      
      

   int connect(const CPacket& pkt) throw ();

      
      
      
      
      
      
      

   void connect(const sockaddr* peer, CHandShake* hs);

      
      
      
      
      
      

   void flush();

      
      
      
      
      
      

   void close();

      
      
      
      
      
      
      

   int send(const char* data, int len);

      
      
      
      
      
      
      

   int recv(char* data, int len);

      
      
      
      
      
      
      
      
      

   int sendmsg(const char* data, int len, int ttl, bool inorder);

      
      
      
      
      
      
      

   int recvmsg(char* data, int len);

      
      
      
      
      
      
      
      
      

   int64_t sendfile(std::fstream& ifs, int64_t& offset, int64_t size, int block = 366000);

      
      
      
      
      
      
      
      
      

   int64_t recvfile(std::fstream& ofs, int64_t& offset, int64_t size, int block = 7320000);

      
      
      
      
      
      
      
      

   void setOpt(UDTOpt optName, const void* optval, int optlen);

      
      
      
      
      
      
      
      

   void getOpt(UDTOpt optName, void* optval, int& optlen);

      
      
      
      
      
      
      

   void sample(CPerfMon* perf, bool clear = true);

private:
   static CUDTUnited s_UDTUnited;               

public:
   static const UDTSOCKET INVALID_SOCK;         
   static const int ERROR;                      

private: 
   UDTSOCKET m_SocketID;                        
   UDTSockType m_iSockType;                     
   UDTSOCKET m_PeerID;				
   static const int m_iVersion;                 

private: 
   int m_iPktSize;                              
   int m_iPayloadSize;                          

private: 
   int m_iMSS;                                  
   bool m_bSynSending;                          
   bool m_bSynRecving;                          
   int m_iFlightFlagSize;                       
   int m_iSndBufSize;                           
   int m_iRcvBufSize;                           
   linger m_Linger;                             
   int m_iUDPSndBufSize;                        
   int m_iUDPRcvBufSize;                        
   int m_iIPversion;                            
   bool m_bRendezvous;                          
   int m_iSndTimeOut;                           
   int m_iRcvTimeOut;                           
   bool m_bReuseAddr;				
   int64_t m_llMaxBW;				

private: 
   CCCVirtualFactory* m_pCCFactory;             
   CCC* m_pCC;                                  
   CCache<CInfoBlock>* m_pCache;		

private: 
   volatile bool m_bListening;                  
   volatile bool m_bConnecting;			
   volatile bool m_bConnected;                  
   volatile bool m_bClosing;                    
   volatile bool m_bShutdown;                   
   volatile bool m_bBroken;                     
   volatile bool m_bPeerHealth;                 
   bool m_bOpened;                              
   int m_iBrokenCounter;			

   int m_iEXPCount;                             
   int m_iBandwidth;                            
   int m_iRTT;                                  
   int m_iRTTVar;                               
   int m_iDeliveryRate;				

   uint64_t m_ullLingerExpiration;		

   CHandShake m_ConnReq;			
   CHandShake m_ConnRes;			
   int64_t m_llLastReqTime;			

private: 
   CSndBuffer* m_pSndBuffer;                    
   CSndLossList* m_pSndLossList;                
   CPktTimeWindow* m_pSndTimeWindow;            

   volatile uint64_t m_ullInterval;             
   uint64_t m_ullTimeDiff;                      

   volatile int m_iFlowWindowSize;              
   volatile double m_dCongestionWindow;         

   volatile int32_t m_iSndLastAck;              
   volatile int32_t m_iSndLastDataAck;          
   volatile int32_t m_iSndCurrSeqNo;            
   int32_t m_iLastDecSeq;                       
   int32_t m_iSndLastAck2;                      
   uint64_t m_ullSndLastAck2Time;               

   int32_t m_iISN;                              

   void CCUpdate();

private: 
   CRcvBuffer* m_pRcvBuffer;                    
   CRcvLossList* m_pRcvLossList;                
   CACKWindow* m_pACKWindow;                    
   CPktTimeWindow* m_pRcvTimeWindow;            

   int32_t m_iRcvLastAck;                       
   uint64_t m_ullLastAckTime;                   
   int32_t m_iRcvLastAckAck;                    
   int32_t m_iAckSeqNo;                         
   int32_t m_iRcvCurrSeqNo;                     

   uint64_t m_ullLastWarningTime;               

   int32_t m_iPeerISN;                          

private: 
   udt_pthread_mutex_t m_ConnectionLock;            

   udt_pthread_cond_t m_SendBlockCond;              
   udt_pthread_mutex_t m_SendBlockLock;             

   udt_pthread_mutex_t m_AckLock;                   

   udt_pthread_cond_t m_RecvDataCond;               
   udt_pthread_mutex_t m_RecvDataLock;              

   udt_pthread_mutex_t m_SendLock;                  
   udt_pthread_mutex_t m_RecvLock;                  

   void initSynch();
   void destroySynch();
   void releaseSynch();

private: 
   void sendCtrl(int pkttype, void* lparam = NULL, void* rparam = NULL, int size = 0);
   void processCtrl(CPacket& ctrlpkt);
   int packData(CPacket& packet, uint64_t& ts);
   int processData(CUnit* unit);
   int listen(sockaddr* addr, CPacket& packet);

private: 
   uint64_t m_StartTime;                        
   int64_t m_llSentTotal;                       
   int64_t m_llRecvTotal;                       
   int m_iSndLossTotal;                         
   int m_iRcvLossTotal;                         
   int m_iRetransTotal;                         
   int m_iSentACKTotal;                         
   int m_iRecvACKTotal;                         
   int m_iSentNAKTotal;                         
   int m_iRecvNAKTotal;                         
   int64_t m_llSndDurationTotal;		

   uint64_t m_LastSampleTime;                   
   int64_t m_llTraceSent;                       
   int64_t m_llTraceRecv;                       
   int m_iTraceSndLoss;                         
   int m_iTraceRcvLoss;                         
   int m_iTraceRetrans;                         
   int m_iSentACK;                              
   int m_iRecvACK;                              
   int m_iSentNAK;                              
   int m_iRecvNAK;                              
   int64_t m_llSndDuration;			
   int64_t m_llSndDurationCounter;		

private: 
   uint64_t m_ullCPUFrequency;                  

   static const int m_iSYNInterval;             
   static const int m_iSelfClockInterval;       

   uint64_t m_ullNextACKTime;			
   uint64_t m_ullNextNAKTime;			

   volatile uint64_t m_ullSYNInt;		
   volatile uint64_t m_ullACKInt;		
   volatile uint64_t m_ullNAKInt;		
   volatile uint64_t m_ullLastRspTime;		

   uint64_t m_ullMinNakInt;			
   uint64_t m_ullMinExpInt;			

   int m_iPktCount;				
   int m_iLightACKCount;			

   uint64_t m_ullTargetTime;			

   void checkTimers();

private: 
   CSndQueue* m_pSndQueue;			
   CRcvQueue* m_pRcvQueue;			
   sockaddr* m_pPeerAddr;			
   uint32_t m_piSelfIP[4];			
   CSNode* m_pSNode;				
   CRNode* m_pRNode;                            

private: 
   std::set<int> m_sPollID;                     
   void addEPoll(const int eid);
   void removeEPoll(const int eid);
};


#endif
