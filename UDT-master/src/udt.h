



#ifndef __UDT_H__
#define __UDT_H__


#ifndef WINDOWS
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
#else
   #include <stdint.h>
   #include <ws2tcpip.h>
   #include <windows.h>
#endif
#include <fstream>
#include <set>
#include <string>
#include <vector>




#ifdef WINDOWS
  typedef SOCKET SYSSOCKET;
  #define UDT_API
#else
   typedef int SYSSOCKET;
   #define UDT_API __attribute__ ((visibility("default")))
#endif

#define NO_BUSY_WAITING

typedef SYSSOCKET UDPSOCKET;
typedef int UDTSOCKET;



typedef std::set<UDTSOCKET> ud_set;
#define UD_CLR(u, uset) ((uset)->erase(u))
#define UD_ISSET(u, uset) ((uset)->find(u) != (uset)->end())
#define UD_SET(u, uset) ((uset)->insert(u))
#define UD_ZERO(uset) ((uset)->clear())

enum EPOLLOpt
{
   
   
   UDT_EPOLL_IN = 0x1,
   UDT_EPOLL_OUT = 0x4,
   UDT_EPOLL_ERR = 0x8
};

enum UDTSTATUS {INIT = 1, OPENED, LISTENING, CONNECTING, CONNECTED, BROKEN, CLOSING, CLOSED, NONEXIST};



enum UDTOpt
{
   UDT_MSS,             
   UDT_SNDSYN,          
   UDT_RCVSYN,          
   UDT_CC,              
   UDT_FC,		
   UDT_SNDBUF,          
   UDT_RCVBUF,          
   UDT_LINGER,          
   UDP_SNDBUF,          
   UDP_RCVBUF,          
   UDT_MAXMSG,          
   UDT_MSGTTL,          
   UDT_RENDEZVOUS,      
   UDT_SNDTIMEO,        
   UDT_RCVTIMEO,        
   UDT_REUSEADDR,	
   UDT_MAXBW,		
   UDT_STATE,		
   UDT_EVENT,		
   UDT_SNDDATA,		
   UDT_RCVDATA		
};



struct CPerfMon
{
   
   int64_t msTimeStamp;                 
   int64_t pktSentTotal;                
   int64_t pktRecvTotal;                
   int pktSndLossTotal;                 
   int pktRcvLossTotal;                 
   int pktRetransTotal;                 
   int pktSentACKTotal;                 
   int pktRecvACKTotal;                 
   int pktSentNAKTotal;                 
   int pktRecvNAKTotal;                 
   int64_t usSndDurationTotal;		

   
   int64_t pktSent;                     
   int64_t pktRecv;                     
   int pktSndLoss;                      
   int pktRcvLoss;                      
   int pktRetrans;                      
   int pktSentACK;                      
   int pktRecvACK;                      
   int pktSentNAK;                      
   int pktRecvNAK;                      
   double mbpsSendRate;                 
   double mbpsRecvRate;                 
   int64_t usSndDuration;		

   
   double usPktSndPeriod;               
   int pktFlowWindow;                   
   int pktCongestionWindow;             
   int pktFlightSize;                   
   double msRTT;                        
   double mbpsBandwidth;                
   int byteAvailSndBuf;                 
   int byteAvailRcvBuf;                 
};



class UDT_API CUDTException
{
public:
   CUDTException(int major = 0, int minor = 0, int err = -1);
   CUDTException(const CUDTException& e);
   virtual ~CUDTException();

      
      
      
      
      
      

   virtual const char* getErrorMessage();

      
      
      
      
      
      

   virtual int getErrorCode() const;

      
      
      
      
      
      

   virtual void clear();

private:
   int m_iMajor;        









   int m_iMinor;		
   int m_iErrno;		
   std::string m_strMsg;	

   std::string m_strAPI;	
   std::string m_strDebug;	

public: 
   static const int SUCCESS;
   static const int ECONNSETUP;
   static const int ENOSERVER;
   static const int ECONNREJ;
   static const int ESOCKFAIL;
   static const int ESECFAIL;
   static const int ECONNFAIL;
   static const int ECONNLOST;
   static const int ENOCONN;
   static const int ERESOURCE;
   static const int ETHREAD;
   static const int ENOBUF;
   static const int EFILE;
   static const int EINVRDOFF;
   static const int ERDPERM;
   static const int EINVWROFF;
   static const int EWRPERM;
   static const int EINVOP;
   static const int EBOUNDSOCK;
   static const int ECONNSOCK;
   static const int EINVPARAM;
   static const int EINVSOCK;
   static const int EUNBOUNDSOCK;
   static const int ENOLISTEN;
   static const int ERDVNOSERV;
   static const int ERDVUNBOUND;
   static const int ESTREAMILL;
   static const int EDGRAMILL;
   static const int EDUPLISTEN;
   static const int ELARGEMSG;
   static const int EINVPOLLID;
   static const int EASYNCFAIL;
   static const int EASYNCSND;
   static const int EASYNCRCV;
   static const int ETIMEOUT;
   static const int EPEERERR;
   static const int EUNKNOWN;
};








namespace UDT
{

typedef CUDTException ERRORINFO;
typedef UDTOpt SOCKOPT;
typedef CPerfMon TRACEINFO;
typedef ud_set UDSET;

UDT_API extern const UDTSOCKET INVALID_SOCK;
#undef ERROR
UDT_API extern const int ERROR;

UDT_API int startup();
UDT_API int cleanup();
UDT_API UDTSOCKET socket(int af, int type, int protocol);
UDT_API int bind(UDTSOCKET u, const struct sockaddr* name, int namelen);
UDT_API int bind2(UDTSOCKET u, UDPSOCKET udpsock);
UDT_API int listen(UDTSOCKET u, int backlog);
UDT_API UDTSOCKET accept(UDTSOCKET u, struct sockaddr* addr, int* addrlen);
UDT_API int connect(UDTSOCKET u, const struct sockaddr* name, int namelen);
UDT_API int flush(UDTSOCKET u);
UDT_API int close(UDTSOCKET u);
UDT_API int getpeername(UDTSOCKET u, struct sockaddr* name, int* namelen);
UDT_API int getsockname(UDTSOCKET u, struct sockaddr* name, int* namelen);
UDT_API int getsockopt(UDTSOCKET u, int level, SOCKOPT optname, void* optval, int* optlen);
UDT_API int setsockopt(UDTSOCKET u, int level, SOCKOPT optname, const void* optval, int optlen);
UDT_API int send(UDTSOCKET u, const char* buf, int len, int flags);
UDT_API int recv(UDTSOCKET u, char* buf, int len, int flags);
UDT_API int sendmsg(UDTSOCKET u, const char* buf, int len, int ttl = -1, bool inorder = false);
UDT_API int recvmsg(UDTSOCKET u, char* buf, int len);
UDT_API int64_t sendfile(UDTSOCKET u, std::fstream& ifs, int64_t& offset, int64_t size, int block = 364000);
UDT_API int64_t recvfile(UDTSOCKET u, std::fstream& ofs, int64_t& offset, int64_t size, int block = 7280000);
UDT_API int64_t sendfile2(UDTSOCKET u, const char* path, int64_t* offset, int64_t size, int block = 364000);
UDT_API int64_t recvfile2(UDTSOCKET u, const char* path, int64_t* offset, int64_t size, int block = 7280000);


UDT_API int select(int nfds, UDSET* readfds, UDSET* writefds, UDSET* exceptfds, const struct timeval* timeout);
UDT_API int selectEx(const std::vector<UDTSOCKET>& fds, std::vector<UDTSOCKET>* readfds,
                     std::vector<UDTSOCKET>* writefds, std::vector<UDTSOCKET>* exceptfds, int64_t msTimeOut);


UDT_API int epoll_update_usock(int eid, UDTSOCKET u, const int* events = NULL);

UDT_API int epoll_verify_usock(int eid, UDTSOCKET u, int* events);

UDT_API int epoll_create();
UDT_API int epoll_add_usock(int eid, UDTSOCKET u, const int* events = NULL);
UDT_API int epoll_add_ssock(int eid, SYSSOCKET s, const int* events = NULL);
UDT_API int epoll_remove_usock(int eid, UDTSOCKET u);
UDT_API int epoll_remove_ssock(int eid, SYSSOCKET s);
UDT_API int epoll_wait(int eid, std::set<UDTSOCKET>* readfds, std::set<UDTSOCKET>* writefds, int64_t msTimeOut,
                       std::set<SYSSOCKET>* lrfds = NULL, std::set<SYSSOCKET>* wrfds = NULL);
UDT_API int epoll_wait2(int eid, UDTSOCKET* readfds, int* rnum, UDTSOCKET* writefds, int* wnum, int64_t msTimeOut,
                        SYSSOCKET* lrfds = NULL, int* lrnum = NULL, SYSSOCKET* lwfds = NULL, int* lwnum = NULL);
UDT_API int epoll_release(int eid);
UDT_API ERRORINFO& getlasterror();
UDT_API int getlasterror_code();
UDT_API const char* getlasterror_desc();
UDT_API int perfmon(UDTSOCKET u, TRACEINFO* perf, bool clear = true);
UDT_API UDTSTATUS getsockstate(UDTSOCKET u);

}  

#endif
