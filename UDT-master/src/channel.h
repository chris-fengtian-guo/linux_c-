



#ifndef __UDT_CHANNEL_H__
#define __UDT_CHANNEL_H__


#include "udt.h"
#include "packet.h"


class CChannel
{
public:
   CChannel();
   CChannel(int version);
   ~CChannel();

      
      
      
      
      
      

   void open(const sockaddr* addr = NULL);

      
      
      
      
      
      

   void open(UDPSOCKET udpsock);

      
      
      
      
      
      

   void close() const;

      
      
      
      
      
      

   int getSndBufSize();

      
      
      
      
      
      

   int getRcvBufSize();

      
      
      
      
      
      

   void setSndBufSize(int size);

      
      
      
      
      
      

   void setRcvBufSize(int size);

      
      
      
      
      
      

   void getSockAddr(sockaddr* addr) const;

      
      
      
      
      
      

   void getPeerAddr(sockaddr* addr) const;

      
      
      
      
      
      
      

   int sendto(const sockaddr* addr, CPacket& packet) const;

      
      
      
      
      
      
      

   int recvfrom(sockaddr* addr, CPacket& packet) const;

private:
   void setUDPSockOpt();

private:
   int m_iIPversion;                    
   int m_iSockAddrSize;                 

   UDPSOCKET m_iSocket;                 

   int m_iSndBufSize;                   
   int m_iRcvBufSize;                   
};


#endif
