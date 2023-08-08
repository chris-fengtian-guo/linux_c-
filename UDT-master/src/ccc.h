




#ifndef __UDT_CCC_H__
#define __UDT_CCC_H__


#include "udt.h"
#include "packet.h"


class UDT_API CCC
{
friend class CUDT;

public:
   CCC();
   virtual ~CCC();

private:
   CCC(const CCC&);
   CCC& operator=(const CCC&) {return *this;}

public:

      
      
      
      
      
      
      

   virtual void init() {}

      
      
      
      
      
      

   virtual void close() {}

      
      
      
      
      
      

   virtual void onACK(int32_t) {}

      
      
      
      
      
      
      

   virtual void onLoss(const int32_t*, int) {}

      
      
      
      
      
      

   virtual void onTimeout() {}

      
      
      
      
      
      
      

   virtual void onPktSent(const CPacket*) {}

      
      
      
      
      
      
      

   virtual void onPktReceived(const CPacket*) {}

      
      
      
      
      
      

   virtual void processCustomMsg(const CPacket*) {}

protected:

      
      
      
      
      
      

   void setACKTimer(int msINT);

      
      
      
      
      
      

   void setACKInterval(int pktINT);

      
      
      
      
      
      

   void setRTO(int usRTO);

      
      
      
      
      
      

   void sendCustomMsg(CPacket& pkt) const;

      
      
      
      
      
      

   const CPerfMon* getPerfInfo();

      
      
      
      
      
      
      

   void setUserParam(const char* param, int size);

private:
   void setMSS(int mss);
   void setMaxCWndSize(int cwnd);
   void setBandwidth(int bw);
   void setSndCurrSeqNo(int32_t seqno);
   void setRcvRate(int rcvrate);
   void setRTT(int rtt);

protected:
   const int32_t& m_iSYNInterval;	

   double m_dPktSndPeriod;              
   double m_dCWndSize;                  

   int m_iBandwidth;			
   double m_dMaxCWndSize;               

   int m_iMSS;				
   int32_t m_iSndCurrSeqNo;		
   int m_iRcvRate;			
   int m_iRTT;				

   char* m_pcParam;			
   int m_iPSize;			

private:
   UDTSOCKET m_UDT;                     

   int m_iACKPeriod;                    
   int m_iACKInterval;                  

   bool m_bUserDefinedRTO;              
   int m_iRTO;                          

   CPerfMon m_PerfInfo;                 
};

class CCCVirtualFactory
{
public:
   virtual ~CCCVirtualFactory() {}

   virtual CCC* create() = 0;
   virtual CCCVirtualFactory* clone() = 0;
};

template <class T>
class CCCFactory: public CCCVirtualFactory
{
public:
   virtual ~CCCFactory() {}

   virtual CCC* create() {return new T;}
   virtual CCCVirtualFactory* clone() {return new CCCFactory<T>;}
};

class CUDTCC: public CCC
{
public:
   CUDTCC();

public:
   virtual void init();
   virtual void onACK(int32_t);
   virtual void onLoss(const int32_t*, int);
   virtual void onTimeout();

private:
   int m_iRCInterval;			
   uint64_t m_LastRCTime;		
   bool m_bSlowStart;			
   int32_t m_iLastAck;			
   bool m_bLoss;			
   int32_t m_iLastDecSeq;		
   double m_dLastDecPeriod;		
   int m_iNAKCount;                     
   int m_iDecRandom;                    
   int m_iAvgNAKNum;                    
   int m_iDecCount;			
};

#endif
