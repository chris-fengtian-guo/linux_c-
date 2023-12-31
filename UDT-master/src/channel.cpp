



#ifndef WINDOWS
   #include <netdb.h>
   #include <arpa/inet.h>
   #include <unistd.h>
   #include <fcntl.h>
   #include <cstring>
   #include <cstdio>
   #include <cerrno>
#else
   #include <winsock2.h>
   #include <ws2tcpip.h>
   #include <wspiapi.h>
#endif
#include "channel.h"
#include "packet.h"

#ifdef WINDOWS
   #define socklen_t int
#endif

#ifndef WINDOWS
   #define NET_ERROR errno
#else
   #define NET_ERROR WSAGetLastError()
#endif


CChannel::CChannel():
m_iIPversion(AF_INET),
m_iSockAddrSize(sizeof(sockaddr_in)),
m_iSocket(),
m_iSndBufSize(65536),
m_iRcvBufSize(65536)
{
}

CChannel::CChannel(int version):
m_iIPversion(version),
m_iSocket(),
m_iSndBufSize(65536),
m_iRcvBufSize(65536)
{
   m_iSockAddrSize = (AF_INET == m_iIPversion) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
}

CChannel::~CChannel()
{
}

void CChannel::open(const sockaddr* addr)
{
   
   m_iSocket = ::socket(m_iIPversion, SOCK_DGRAM, 0);

   #ifdef WINDOWS
      if (INVALID_SOCKET == m_iSocket)
   #else
      if (m_iSocket < 0)
   #endif
      throw CUDTException(1, 0, NET_ERROR);

   if (NULL != addr)
   {
      socklen_t namelen = m_iSockAddrSize;

      if (0 != ::bind(m_iSocket, addr, namelen))
         throw CUDTException(1, 3, NET_ERROR);
   }
   else
   {
      
      addrinfo hints;
      addrinfo* res;

      memset(&hints, 0, sizeof(struct addrinfo));

      hints.ai_flags = AI_PASSIVE;
      hints.ai_family = m_iIPversion;
      hints.ai_socktype = SOCK_DGRAM;

      if (0 != ::getaddrinfo(NULL, "0", &hints, &res))
         throw CUDTException(1, 3, NET_ERROR);

      if (0 != ::bind(m_iSocket, res->ai_addr, res->ai_addrlen))
         throw CUDTException(1, 3, NET_ERROR);

      ::freeaddrinfo(res);
   }

   setUDPSockOpt();
}

void CChannel::open(UDPSOCKET udpsock)
{
   m_iSocket = udpsock;
   setUDPSockOpt();
}

void CChannel::setUDPSockOpt()
{
   #if defined(BSD) || defined(MACOSX)
      
      int maxsize = 64000;
      if (0 != ::setsockopt(m_iSocket, SOL_SOCKET, SO_RCVBUF, (char*)&m_iRcvBufSize, sizeof(int)))
         ::setsockopt(m_iSocket, SOL_SOCKET, SO_RCVBUF, (char*)&maxsize, sizeof(int));
      if (0 != ::setsockopt(m_iSocket, SOL_SOCKET, SO_SNDBUF, (char*)&m_iSndBufSize, sizeof(int)))
         ::setsockopt(m_iSocket, SOL_SOCKET, SO_SNDBUF, (char*)&maxsize, sizeof(int));
   #else
      
      if ((0 != ::setsockopt(m_iSocket, SOL_SOCKET, SO_RCVBUF, (char*)&m_iRcvBufSize, sizeof(int))) ||
          (0 != ::setsockopt(m_iSocket, SOL_SOCKET, SO_SNDBUF, (char*)&m_iSndBufSize, sizeof(int))))
         throw CUDTException(1, 3, NET_ERROR);
   #endif

   timeval tv;
   tv.tv_sec = 0;
   #if defined (BSD) || defined (MACOSX)
      
      
      tv.tv_usec = 10000;
   #else
      tv.tv_usec = 100;
   #endif

   #ifdef UNIX
      
      
      int opts = ::fcntl(m_iSocket, F_GETFL);
      if (-1 == ::fcntl(m_iSocket, F_SETFL, opts | O_NONBLOCK))
         throw CUDTException(1, 3, NET_ERROR);
   #elif WINDOWS
      DWORD ot = 1; 
      if (0 != ::setsockopt(m_iSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&ot, sizeof(DWORD)))
         throw CUDTException(1, 3, NET_ERROR);
   #else
      
      if (0 != ::setsockopt(m_iSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(timeval)))
         throw CUDTException(1, 3, NET_ERROR);
   #endif
}

void CChannel::close() const
{
   #ifndef WINDOWS
      ::close(m_iSocket);
   #else
      ::closesocket(m_iSocket);
   #endif
}

int CChannel::getSndBufSize()
{
   socklen_t size = sizeof(socklen_t);
   ::getsockopt(m_iSocket, SOL_SOCKET, SO_SNDBUF, (char *)&m_iSndBufSize, &size);
   return m_iSndBufSize;
}

int CChannel::getRcvBufSize()
{
   socklen_t size = sizeof(socklen_t);
   ::getsockopt(m_iSocket, SOL_SOCKET, SO_RCVBUF, (char *)&m_iRcvBufSize, &size);
   return m_iRcvBufSize;
}

void CChannel::setSndBufSize(int size)
{
   m_iSndBufSize = size;
}

void CChannel::setRcvBufSize(int size)
{
   m_iRcvBufSize = size;
}

void CChannel::getSockAddr(sockaddr* addr) const
{
   socklen_t namelen = m_iSockAddrSize;
   ::getsockname(m_iSocket, addr, &namelen);
}

void CChannel::getPeerAddr(sockaddr* addr) const
{
   socklen_t namelen = m_iSockAddrSize;
   ::getpeername(m_iSocket, addr, &namelen);
}

int CChannel::sendto(const sockaddr* addr, CPacket& packet) const
{
   
   if (packet.getFlag())
      for (int i = 0, n = packet.getLength() / 4; i < n; ++ i)
         *((uint32_t *)packet.m_pcData + i) = htonl(*((uint32_t *)packet.m_pcData + i));

   
   
   
   uint32_t* p = packet.m_nHeader;
   for (int j = 0; j < 4; ++ j)
   {
      *p = htonl(*p);
      ++ p;
   }

   #ifndef WINDOWS
      msghdr mh;
      mh.msg_name = (sockaddr*)addr;
      mh.msg_namelen = m_iSockAddrSize;
      mh.msg_iov = (iovec*)packet.m_PacketVector;
      mh.msg_iovlen = 2;
      mh.msg_control = NULL;
      mh.msg_controllen = 0;
      mh.msg_flags = 0;

      int res = ::sendmsg(m_iSocket, &mh, 0);
   #else
      DWORD size = CPacket::m_iPktHdrSize + packet.getLength();
      int addrsize = m_iSockAddrSize;
      int res = ::WSASendTo(m_iSocket, (LPWSABUF)packet.m_PacketVector, 2, &size, 0, addr, addrsize, NULL, NULL);
      res = (0 == res) ? size : -1;
   #endif

   
   
   
   p = packet.m_nHeader;
   for (int k = 0; k < 4; ++ k)
   {
      *p = ntohl(*p);
       ++ p;
   }

   if (packet.getFlag())
   {
      for (int l = 0, n = packet.getLength() / 4; l < n; ++ l)
         *((uint32_t *)packet.m_pcData + l) = ntohl(*((uint32_t *)packet.m_pcData + l));
   }

   return res;
}

int CChannel::recvfrom(sockaddr* addr, CPacket& packet) const
{
   #ifndef WINDOWS
      msghdr mh;
      mh.msg_name = addr;
      mh.msg_namelen = m_iSockAddrSize;
      mh.msg_iov = packet.m_PacketVector;
      mh.msg_iovlen = 2;
      mh.msg_control = NULL;
      mh.msg_controllen = 0;
      mh.msg_flags = 0;

      #ifdef UNIX
         fd_set set;
         timeval tv;
         FD_ZERO(&set);
         FD_SET(m_iSocket, &set);
         tv.tv_sec = 0;
         tv.tv_usec = 10000;
         ::select(m_iSocket+1, &set, NULL, &set, &tv);
      #endif

      int res = ::recvmsg(m_iSocket, &mh, 0);
   #else
      DWORD size = CPacket::m_iPktHdrSize + packet.getLength();
      DWORD flag = 0;
      int addrsize = m_iSockAddrSize;

      int res = ::WSARecvFrom(m_iSocket, (LPWSABUF)packet.m_PacketVector, 2, &size, &flag, addr, &addrsize, NULL, NULL);
      res = (0 == res) ? size : -1;
   #endif

   if (res <= 0)
   {
      packet.setLength(-1);
      return -1;
   }

   packet.setLength(res - CPacket::m_iPktHdrSize);

   
   
   
   uint32_t* p = packet.m_nHeader;
   for (int i = 0; i < 4; ++ i)
   {
      *p = ntohl(*p);
      ++ p;
   }

   if (packet.getFlag())
   {
      for (int j = 0, n = packet.getLength() / 4; j < n; ++ j)
         *((uint32_t *)packet.m_pcData + j) = ntohl(*((uint32_t *)packet.m_pcData + j));
   }

   return packet.getLength();
}
