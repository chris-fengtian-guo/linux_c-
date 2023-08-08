



#ifndef __UDT_EPOLL_H__
#define __UDT_EPOLL_H__


#include <map>
#include <set>
#include "udt.h"


struct CEPollDesc
{
   int m_iID;                                
   std::set<UDTSOCKET> m_sUDTSocksOut;       
   std::set<UDTSOCKET> m_sUDTSocksIn;        
   std::set<UDTSOCKET> m_sUDTSocksEx;        

   int m_iLocalID;                           
   std::set<SYSSOCKET> m_sLocals;            

   std::set<UDTSOCKET> m_sUDTWrites;         
   std::set<UDTSOCKET> m_sUDTReads;          
   std::set<UDTSOCKET> m_sUDTExcepts;        
};

class CEPoll
{
friend class CUDT;
friend class CRendezvousQueue;

public:
   CEPoll();
   ~CEPoll();

public: 

      
      
      
      
      
      

   int create();

      
      
      
      
      
      
      
      

   int add_usock(const int eid, const UDTSOCKET& u, const int* events = NULL);

   
   int update_usock(const int eid, const UDTSOCKET& u, const int* events = NULL);
   
   int verify_usock(const int eid, const UDTSOCKET& u, int* events);

      
      
      
      
      
      
      
      

   int add_ssock(const int eid, const SYSSOCKET& s, const int* events = NULL);

      
      
      
      
      
      
      

   int remove_usock(const int eid, const UDTSOCKET& u);

      
      
      
      
      
      
      

   int remove_ssock(const int eid, const SYSSOCKET& s);

      
      
      
      
      
      
      
      
      
      
      

   int wait(const int eid, std::set<UDTSOCKET>* readfds, std::set<UDTSOCKET>* writefds, int64_t msTimeOut, std::set<SYSSOCKET>* lrfds, std::set<SYSSOCKET>* lwfds);

      
      
      
      
      
      

   int release(const int eid);

public: 

      
      
      
      
      
      
      
      
      

   int update_events(const UDTSOCKET& uid, std::set<int>& eids, int events, bool enable);

private:
   int m_iIDSeed;                            
   udt_pthread_mutex_t m_SeedLock;

   std::map<int, CEPollDesc> m_mPolls;       
   udt_pthread_mutex_t m_EPollLock;
};


#endif
