












































































































#include <cstring>
#include "packet.h"


const int CPacket::m_iPktHdrSize = 16;
const int CHandShake::m_iContentSize = 48;



CPacket::CPacket():
m_iSeqNo((int32_t&)(m_nHeader[0])),
m_iMsgNo((int32_t&)(m_nHeader[1])),
m_iTimeStamp((int32_t&)(m_nHeader[2])),
m_iID((int32_t&)(m_nHeader[3])),
m_pcData((char*&)(m_PacketVector[1].iov_base)),
__pad()
{
   for (int i = 0; i < 4; ++ i)
      m_nHeader[i] = 0;
   m_PacketVector[0].iov_base = (char *)m_nHeader;
   m_PacketVector[0].iov_len = CPacket::m_iPktHdrSize;
   m_PacketVector[1].iov_base = NULL;
   m_PacketVector[1].iov_len = 0;
}

CPacket::~CPacket()
{
}

int CPacket::getLength() const
{
   return m_PacketVector[1].iov_len;
}

void CPacket::setLength(int len)
{
   m_PacketVector[1].iov_len = len;
}

void CPacket::pack(int pkttype, void* lparam, void* rparam, int size)
{
   
   m_nHeader[0] = 0x80000000 | (pkttype << 16);

   
   switch (pkttype)
   {
   case 2: 
      
      if (NULL != lparam)
         m_nHeader[1] = *(int32_t *)lparam;

      
      
      m_PacketVector[1].iov_base = (char *)rparam;
      m_PacketVector[1].iov_len = size;

      break;

   case 6: 
      
      m_nHeader[1] = *(int32_t *)lparam;

      
      
      m_PacketVector[1].iov_base = (char *)&__pad; 
      m_PacketVector[1].iov_len = 4; 

      break;

   case 3: 
      
      m_PacketVector[1].iov_base = (char *)rparam;
      m_PacketVector[1].iov_len = size;

      break;

   case 4: 
      
      
      m_PacketVector[1].iov_base = (char *)&__pad; 
      m_PacketVector[1].iov_len = 4; 
  
      break;

   case 1: 
      
      
      m_PacketVector[1].iov_base = (char *)&__pad; 
      m_PacketVector[1].iov_len = 4; 

      break;

   case 0: 
      
      m_PacketVector[1].iov_base = (char *)rparam;
      m_PacketVector[1].iov_len = size; 

      break;

   case 5: 
      
      
      m_PacketVector[1].iov_base = (char *)&__pad; 
      m_PacketVector[1].iov_len = 4; 

      break;

   case 7: 
      
      m_nHeader[1] = *(int32_t *)lparam;

      
      m_PacketVector[1].iov_base = (char *)rparam;
      m_PacketVector[1].iov_len = size;

      break;

   case 8: 
      
      m_nHeader[1] = *(int32_t *)lparam;

      
      
      m_PacketVector[1].iov_base = (char *)&__pad; 
      m_PacketVector[1].iov_len = 4; 

      break;

   case 32767: 
      
      
      
      m_nHeader[0] |= *(int32_t *)lparam;

      if (NULL != rparam)
      {
         m_PacketVector[1].iov_base = (char *)rparam;
         m_PacketVector[1].iov_len = size;
      }
      else
      {
         m_PacketVector[1].iov_base = (char *)&__pad;
         m_PacketVector[1].iov_len = 4;
      }

      break;

   default:
      break;
   }
}

iovec* CPacket::getPacketVector()
{
   return m_PacketVector;
}

int CPacket::getFlag() const
{
   
   return m_nHeader[0] >> 31;
}

int CPacket::getType() const
{
   
   return (m_nHeader[0] >> 16) & 0x00007FFF;
}

int CPacket::getExtendedType() const
{
   
   return m_nHeader[0] & 0x0000FFFF;
}

int32_t CPacket::getAckSeqNo() const
{
   
   return m_nHeader[1];
}

int CPacket::getMsgBoundary() const
{
   
   return m_nHeader[1] >> 30;
}

bool CPacket::getMsgOrderFlag() const
{
   
   return (1 == ((m_nHeader[1] >> 29) & 1));
}

int32_t CPacket::getMsgSeq() const
{
   
   return m_nHeader[1] & 0x1FFFFFFF;
}

CPacket* CPacket::clone() const
{
   CPacket* pkt = new CPacket;
   memcpy(pkt->m_nHeader, m_nHeader, m_iPktHdrSize);
   pkt->m_pcData = new char[m_PacketVector[1].iov_len];
   memcpy(pkt->m_pcData, m_pcData, m_PacketVector[1].iov_len);
   pkt->m_PacketVector[1].iov_len = m_PacketVector[1].iov_len;

   return pkt;
}

CHandShake::CHandShake():
m_iVersion(0),
m_iType(0),
m_iISN(0),
m_iMSS(0),
m_iFlightFlagSize(0),
m_iReqType(0),
m_iID(0),
m_iCookie(0)
{
   for (int i = 0; i < 4; ++ i)
      m_piPeerIP[i] = 0;
}

int CHandShake::serialize(char* buf, int& size)
{
   if (size < m_iContentSize)
      return -1;

   int32_t* p = (int32_t*)buf;
   *p++ = m_iVersion;
   *p++ = m_iType;
   *p++ = m_iISN;
   *p++ = m_iMSS;
   *p++ = m_iFlightFlagSize;
   *p++ = m_iReqType;
   *p++ = m_iID;
   *p++ = m_iCookie;
   for (int i = 0; i < 4; ++ i)
      *p++ = m_piPeerIP[i];

   size = m_iContentSize;

   return 0;
}

int CHandShake::deserialize(const char* buf, int size)
{
   if (size < m_iContentSize)
      return -1;

   int32_t* p = (int32_t*)buf;
   m_iVersion = *p++;
   m_iType = *p++;
   m_iISN = *p++;
   m_iMSS = *p++;
   m_iFlightFlagSize = *p++;
   m_iReqType = *p++;
   m_iID = *p++;
   m_iCookie = *p++;
   for (int i = 0; i < 4; ++ i)
      m_piPeerIP[i] = *p++;

   return 0;
}
