



#ifndef __UDT_WINDOW_H__
#define __UDT_WINDOW_H__


#ifndef WINDOWS
   #include <sys/time.h>
   #include <time.h>
#endif
#include "udt.h"


class CACKWindow
{
public:
   CACKWindow(int size = 1024);
   ~CACKWindow();

      
      
      
      
      
      
      

   void store(int32_t seq, int32_t ack);

      
      
      
      
      
      
      

   int acknowledge(int32_t seq, int32_t& ack);

private:
   int32_t* m_piACKSeqNo;       
   int32_t* m_piACK;            
   uint64_t* m_pTimeStamp;      

   int m_iSize;                 
   int m_iHead;                 
   int m_iTail;                 

private:
   CACKWindow(const CACKWindow&);
   CACKWindow& operator=(const CACKWindow&);
};



class CPktTimeWindow
{
public:
   CPktTimeWindow(int asize = 16, int psize = 16);
   ~CPktTimeWindow();

      
      
      
      
      
      

   int getMinPktSndInt() const;

      
      
      
      
      
      

   int getPktRcvSpeed() const;

      
      
      
      
      
      

   int getBandwidth() const;

      
      
      
      
      
      

   void onPktSent(int currtime);

      
      
      
      
      
      

   void onPktArrival();

      
      
      
      
      
      

   void probe1Arrival();

      
      
      
      
      
      

   void probe2Arrival();

private:
   int m_iAWSize;               
   int* m_piPktWindow;          
   int* m_piPktReplica;
   int m_iPktWindowPtr;         

   int m_iPWSize;               
   int* m_piProbeWindow;        
   int* m_piProbeReplica;
   int m_iProbeWindowPtr;       

   int m_iLastSentTime;         
   int m_iMinPktSndInt;         

   uint64_t m_LastArrTime;      
   uint64_t m_CurrArrTime;      
   uint64_t m_ProbeTime;        

private:
   CPktTimeWindow(const CPktTimeWindow&);
   CPktTimeWindow &operator=(const CPktTimeWindow&);
};


#endif
