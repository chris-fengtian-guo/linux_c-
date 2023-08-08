



#ifndef __UDT_PACKET_H__
#define __UDT_PACKET_H__


#include "udt.h"

#ifdef WINDOWS
   struct iovec
   {
      int iov_len;
      char* iov_base;
   };
#endif

class CChannel;

class CPacket
{
friend class CChannel;
friend class CSndQueue;
friend class CRcvQueue;

public:
   int32_t& m_iSeqNo;                   
   int32_t& m_iMsgNo;                   
   int32_t& m_iTimeStamp;               
   int32_t& m_iID;			
   char*& m_pcData;                     

   static const int m_iPktHdrSize;	

public:
   CPacket();
   ~CPacket();

      
      
      
      
      
      

   int getLength() const;

      
      
      
      
      
      

   void setLength(int len);

      
      
      
      
      
      
      
      
      

   void pack(int pkttype, void* lparam = NULL, void* rparam = NULL, int size = 0);

      
      
      
      
      
      

   iovec* getPacketVector();

      
      
      
      
      
      

   int getFlag() const;

      
      
      
      
      
      

   int getType() const;

      
      
      
      
      
      

   int getExtendedType() const;

      
      
      
      
      
      

   int32_t getAckSeqNo() const;

      
      
      
      
      
      

   int getMsgBoundary() const;

      
      
      
      
      
      

   bool getMsgOrderFlag() const;

      
      
      
      
      
      

   int32_t getMsgSeq() const;

      
      
      
      
      
      

   CPacket* clone() const;

protected:
   uint32_t m_nHeader[4];               
   iovec m_PacketVector[2];             

   int32_t __pad;

protected:
   CPacket& operator=(const CPacket&);
};



class CHandShake
{
public:
   CHandShake();

   int serialize(char* buf, int& size);
   int deserialize(const char* buf, int size);

public:
   static const int m_iContentSize;	

public:
   int32_t m_iVersion;          
   int32_t m_iType;             
   int32_t m_iISN;              
   int32_t m_iMSS;              
   int32_t m_iFlightFlagSize;   
   int32_t m_iReqType;          
   int32_t m_iID;		
   int32_t m_iCookie;		
   uint32_t m_piPeerIP[4];	
};


#endif
