



#include <cstring>
#include <cmath>
#include "buffer.h"

using namespace std;

CSndBuffer::CSndBuffer(int size, int mss):
m_BufLock(),
m_pBlock(NULL),
m_pFirstBlock(NULL),
m_pCurrBlock(NULL),
m_pLastBlock(NULL),
m_pBuffer(NULL),
m_iNextMsgNo(1),
m_iSize(size),
m_iMSS(mss),
m_iCount(0)
{
   
   m_pBuffer = new Buffer;
   m_pBuffer->m_pcData = new char [m_iSize * m_iMSS];
   m_pBuffer->m_iSize = m_iSize;
   m_pBuffer->m_pNext = NULL;

   
   m_pBlock = new Block;
   Block* pb = m_pBlock;
   for (int i = 1; i < m_iSize; ++ i)
   {
      pb->m_pNext = new Block;
      pb->m_iMsgNo = 0;
      pb = pb->m_pNext;
   }
   pb->m_pNext = m_pBlock;

   pb = m_pBlock;
   char* pc = m_pBuffer->m_pcData;
   for (int i = 0; i < m_iSize; ++ i)
   {
      pb->m_pcData = pc;
      pb = pb->m_pNext;
      pc += m_iMSS;
   }

   m_pFirstBlock = m_pCurrBlock = m_pLastBlock = m_pBlock;

   #ifndef WINDOWS
      pthread_mutex_init(&m_BufLock, NULL);
   #else
      m_BufLock = CreateMutex(NULL, false, NULL);
   #endif
}

CSndBuffer::~CSndBuffer()
{
   Block* pb = m_pBlock->m_pNext;
   while (pb != m_pBlock)
   {
      Block* temp = pb;
      pb = pb->m_pNext;
      delete temp;
   }
   delete m_pBlock;

   while (m_pBuffer != NULL)
   {
      Buffer* temp = m_pBuffer;
      m_pBuffer = m_pBuffer->m_pNext;
      delete [] temp->m_pcData;
      delete temp;
   }

   #ifndef WINDOWS
      pthread_mutex_destroy(&m_BufLock);
   #else
      CloseHandle(m_BufLock);
   #endif
}

void CSndBuffer::addBuffer(const char* data, int len, int ttl, bool order)
{
   int size = len / m_iMSS;
   if ((len % m_iMSS) != 0)
      size ++;

   
   while (size + m_iCount >= m_iSize)
      increase();

   uint64_t time = CTimer::getTime();
   int32_t inorder = order;
   inorder <<= 29;

   Block* s = m_pLastBlock;
   for (int i = 0; i < size; ++ i)
   {
      int pktlen = len - i * m_iMSS;
      if (pktlen > m_iMSS)
         pktlen = m_iMSS;

      memcpy(s->m_pcData, data + i * m_iMSS, pktlen);
      s->m_iLength = pktlen;

      s->m_iMsgNo = m_iNextMsgNo | inorder;
      if (i == 0)
         s->m_iMsgNo |= 0x80000000;
      if (i == size - 1)
         s->m_iMsgNo |= 0x40000000;

      s->m_OriginTime = time;
      s->m_iTTL = ttl;

      s = s->m_pNext;
   }
   m_pLastBlock = s;

   CGuard::enterCS(m_BufLock);
   m_iCount += size;
   CGuard::leaveCS(m_BufLock);

   m_iNextMsgNo ++;
   if (m_iNextMsgNo == CMsgNo::m_iMaxMsgNo)
      m_iNextMsgNo = 1;
}

int CSndBuffer::addBufferFromFile(fstream& ifs, int len)
{
   int size = len / m_iMSS;
   if ((len % m_iMSS) != 0)
      size ++;

   
   while (size + m_iCount >= m_iSize)
      increase();

   Block* s = m_pLastBlock;
   int total = 0;
   for (int i = 0; i < size; ++ i)
   {
      if (ifs.bad() || ifs.fail() || ifs.eof())
         break;

      int pktlen = len - i * m_iMSS;
      if (pktlen > m_iMSS)
         pktlen = m_iMSS;

      ifs.read(s->m_pcData, pktlen);
      if ((pktlen = ifs.gcount()) <= 0)
         break;

      
      s->m_iMsgNo = m_iNextMsgNo | 0x20000000;
      if (i == 0)
         s->m_iMsgNo |= 0x80000000;
      if (i == size - 1)
         s->m_iMsgNo |= 0x40000000;

      s->m_iLength = pktlen;
      s->m_iTTL = -1;
      s = s->m_pNext;

      total += pktlen;
   }
   m_pLastBlock = s;

   CGuard::enterCS(m_BufLock);
   m_iCount += size;
   CGuard::leaveCS(m_BufLock);

   m_iNextMsgNo ++;
   if (m_iNextMsgNo == CMsgNo::m_iMaxMsgNo)
      m_iNextMsgNo = 1;

   return total;
}

int CSndBuffer::readData(char** data, int32_t& msgno)
{
   
   if (m_pCurrBlock == m_pLastBlock)
      return 0;

   *data = m_pCurrBlock->m_pcData;
   int readlen = m_pCurrBlock->m_iLength;
   msgno = m_pCurrBlock->m_iMsgNo;

   m_pCurrBlock = m_pCurrBlock->m_pNext;

   return readlen;
}

int CSndBuffer::readData(char** data, const int offset, int32_t& msgno, int& msglen)
{
   CGuard bufferguard(m_BufLock);

   Block* p = m_pFirstBlock;

   for (int i = 0; i < offset; ++ i)
      p = p->m_pNext;

   if ((p->m_iTTL >= 0) && ((CTimer::getTime() - p->m_OriginTime) / 1000 > (uint64_t)p->m_iTTL))
   {
      msgno = p->m_iMsgNo & 0x1FFFFFFF;

      msglen = 1;
      p = p->m_pNext;
      bool move = false;
      while (msgno == (p->m_iMsgNo & 0x1FFFFFFF))
      {
         if (p == m_pCurrBlock)
            move = true;
         p = p->m_pNext;
         if (move)
            m_pCurrBlock = p;
         msglen ++;
      }

      return -1;
   }

   *data = p->m_pcData;
   int readlen = p->m_iLength;
   msgno = p->m_iMsgNo;

   return readlen;
}

void CSndBuffer::ackData(int offset)
{
   CGuard bufferguard(m_BufLock);

   for (int i = 0; i < offset; ++ i)
      m_pFirstBlock = m_pFirstBlock->m_pNext;

   m_iCount -= offset;

   CTimer::triggerEvent();
}

int CSndBuffer::getCurrBufSize() const
{
   return m_iCount;
}

void CSndBuffer::increase()
{
   int unitsize = m_pBuffer->m_iSize;

   
   Buffer* nbuf = NULL;
   try
   {
      nbuf  = new Buffer;
      nbuf->m_pcData = new char [unitsize * m_iMSS];
   }
   catch (...)
   {
      delete nbuf;
      throw CUDTException(3, 2, 0);
   }
   nbuf->m_iSize = unitsize;
   nbuf->m_pNext = NULL;

   
   Buffer* p = m_pBuffer;
   while (NULL != p->m_pNext)
      p = p->m_pNext;
   p->m_pNext = nbuf;

   
   Block* nblk = NULL;
   try
   {
      nblk = new Block;
   }
   catch (...)
   {
      delete nblk;
      throw CUDTException(3, 2, 0);
   }
   Block* pb = nblk;
   for (int i = 1; i < unitsize; ++ i)
   {
      pb->m_pNext = new Block;
      pb = pb->m_pNext;
   }

   
   pb->m_pNext = m_pLastBlock->m_pNext;
   m_pLastBlock->m_pNext = nblk;

   pb = nblk;
   char* pc = nbuf->m_pcData;
   for (int i = 0; i < unitsize; ++ i)
   {
      pb->m_pcData = pc;
      pb = pb->m_pNext;
      pc += m_iMSS;
   }

   m_iSize += unitsize;
}



CRcvBuffer::CRcvBuffer(CUnitQueue* queue, int bufsize):
m_pUnit(NULL),
m_iSize(bufsize),
m_pUnitQueue(queue),
m_iStartPos(0),
m_iLastAckPos(0),
m_iMaxPos(0),
m_iNotch(0)
{
   m_pUnit = new CUnit* [m_iSize];
   for (int i = 0; i < m_iSize; ++ i)
      m_pUnit[i] = NULL;
}

CRcvBuffer::~CRcvBuffer()
{
   for (int i = 0; i < m_iSize; ++ i)
   {
      if (NULL != m_pUnit[i])
      {
         m_pUnit[i]->m_iFlag = 0;
         -- m_pUnitQueue->m_iCount;
      }
   }

   delete [] m_pUnit;
}

int CRcvBuffer::addData(CUnit* unit, int offset)
{
   int pos = (m_iLastAckPos + offset) % m_iSize;
   if (offset > m_iMaxPos)
      m_iMaxPos = offset;

   if (NULL != m_pUnit[pos])
      return -1;

   m_pUnit[pos] = unit;

   unit->m_iFlag = 1;
   ++ m_pUnitQueue->m_iCount;

   return 0;
}

int CRcvBuffer::readBuffer(char* data, int len)
{
   int p = m_iStartPos;
   int lastack = m_iLastAckPos;
   int rs = len;

   while ((p != lastack) && (rs > 0))
   {
      int unitsize = m_pUnit[p]->m_Packet.getLength() - m_iNotch;
      if (unitsize > rs)
         unitsize = rs;

      memcpy(data, m_pUnit[p]->m_Packet.m_pcData + m_iNotch, unitsize);
      data += unitsize;

      if ((rs > unitsize) || (rs == m_pUnit[p]->m_Packet.getLength() - m_iNotch))
      {
         CUnit* tmp = m_pUnit[p];
         m_pUnit[p] = NULL;
         tmp->m_iFlag = 0;
         -- m_pUnitQueue->m_iCount;

         if (++ p == m_iSize)
            p = 0;

         m_iNotch = 0;
      }
      else
         m_iNotch += rs;

      rs -= unitsize;
   }

   m_iStartPos = p;
   return len - rs;
}

int CRcvBuffer::readBufferToFile(fstream& ofs, int len)
{
   int p = m_iStartPos;
   int lastack = m_iLastAckPos;
   int rs = len;

   while ((p != lastack) && (rs > 0))
   {
      int unitsize = m_pUnit[p]->m_Packet.getLength() - m_iNotch;
      if (unitsize > rs)
         unitsize = rs;

      ofs.write(m_pUnit[p]->m_Packet.m_pcData + m_iNotch, unitsize);
      if (ofs.fail())
         break;

      if ((rs > unitsize) || (rs == m_pUnit[p]->m_Packet.getLength() - m_iNotch))
      {
         CUnit* tmp = m_pUnit[p];
         m_pUnit[p] = NULL;
         tmp->m_iFlag = 0;
         -- m_pUnitQueue->m_iCount;

         if (++ p == m_iSize)
            p = 0;

         m_iNotch = 0;
      }
      else
         m_iNotch += rs;

      rs -= unitsize;
   }

   m_iStartPos = p;

   return len - rs;
}

void CRcvBuffer::ackData(int len)
{
   m_iLastAckPos = (m_iLastAckPos + len) % m_iSize;
   m_iMaxPos -= len;
   if (m_iMaxPos < 0)
      m_iMaxPos = 0;

   CTimer::triggerEvent();
}

int CRcvBuffer::getAvailBufSize() const
{
   
   return m_iSize - getRcvDataSize() - 1;
}

int CRcvBuffer::getRcvDataSize() const
{
   if (m_iLastAckPos >= m_iStartPos)
      return m_iLastAckPos - m_iStartPos;

   return m_iSize + m_iLastAckPos - m_iStartPos;
}

void CRcvBuffer::dropMsg(int32_t msgno)
{
   for (int i = m_iStartPos, n = (m_iLastAckPos + m_iMaxPos) % m_iSize; i != n; i = (i + 1) % m_iSize)
      if ((NULL != m_pUnit[i]) && (msgno == m_pUnit[i]->m_Packet.m_iMsgNo))
         m_pUnit[i]->m_iFlag = 3;
}

int CRcvBuffer::readMsg(char* data, int len)
{
   int p, q;
   bool passack;
   if (!scanMsg(p, q, passack))
      return 0;

   int rs = len;
   while (p != (q + 1) % m_iSize)
   {
      int unitsize = m_pUnit[p]->m_Packet.getLength();
      if ((rs >= 0) && (unitsize > rs))
         unitsize = rs;

      if (unitsize > 0)
      {
         memcpy(data, m_pUnit[p]->m_Packet.m_pcData, unitsize);
         data += unitsize;
         rs -= unitsize;
      }

      if (!passack)
      {
         CUnit* tmp = m_pUnit[p];
         m_pUnit[p] = NULL;
         tmp->m_iFlag = 0;
         -- m_pUnitQueue->m_iCount;
      }
      else
         m_pUnit[p]->m_iFlag = 2;

      if (++ p == m_iSize)
         p = 0;
   }

   if (!passack)
      m_iStartPos = (q + 1) % m_iSize;

   return len - rs;
}

int CRcvBuffer::getRcvMsgNum()
{
   int p, q;
   bool passack;
   return scanMsg(p, q, passack) ? 1 : 0;
}

bool CRcvBuffer::scanMsg(int& p, int& q, bool& passack)
{
   
   if ((m_iStartPos == m_iLastAckPos) && (m_iMaxPos <= 0))
      return false;

   
   while (m_iStartPos != m_iLastAckPos)
   {
      if (NULL == m_pUnit[m_iStartPos])
      {
         if (++ m_iStartPos == m_iSize)
            m_iStartPos = 0;
         continue;
      }

      if ((1 == m_pUnit[m_iStartPos]->m_iFlag) && (m_pUnit[m_iStartPos]->m_Packet.getMsgBoundary() > 1))
      {
         bool good = true;

         
         for (int i = m_iStartPos; i != m_iLastAckPos;)
         {
            if ((NULL == m_pUnit[i]) || (1 != m_pUnit[i]->m_iFlag))
            {
               good = false;
               break;
            }

            if ((m_pUnit[i]->m_Packet.getMsgBoundary() == 1) || (m_pUnit[i]->m_Packet.getMsgBoundary() == 3))
               break;

            if (++ i == m_iSize)
               i = 0;
         }

         if (good)
            break;
      }

      CUnit* tmp = m_pUnit[m_iStartPos];
      m_pUnit[m_iStartPos] = NULL;
      tmp->m_iFlag = 0;
      -- m_pUnitQueue->m_iCount;

      if (++ m_iStartPos == m_iSize)
         m_iStartPos = 0;
   }

   p = -1;                  
   q = m_iStartPos;         
   passack = m_iStartPos == m_iLastAckPos;
   bool found = false;

   
   for (int i = 0, n = m_iMaxPos + getRcvDataSize(); i <= n; ++ i)
   {
      if ((NULL != m_pUnit[q]) && (1 == m_pUnit[q]->m_iFlag))
      {
         switch (m_pUnit[q]->m_Packet.getMsgBoundary())
         {
         case 3: 
            p = q;
            found = true;
            break;

         case 2: 
            p = q;
            break;

         case 1: 
            if (p != -1)
               found = true;
         }
      }
      else
      {
         
         p = -1;
      }

      if (found)
      {
         
         if (!passack || !m_pUnit[q]->m_Packet.getMsgOrderFlag())
            break;

         found = false;
      }

      if (++ q == m_iSize)
         q = 0;

      if (q == m_iLastAckPos)
         passack = true;
   }

   
   if (!found)
   {
      
      if ((p != -1) && ((q + 1) % m_iSize == p))
         found = true;
   }

   return found;
}
