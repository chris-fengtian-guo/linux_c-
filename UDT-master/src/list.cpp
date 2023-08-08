



#include "list.h"

CSndLossList::CSndLossList(int size):
m_piData1(NULL),
m_piData2(NULL),
m_piNext(NULL),
m_iHead(-1),
m_iLength(0),
m_iSize(size),
m_iLastInsertPos(-1),
m_ListLock()
{
   m_piData1 = new int32_t [m_iSize];
   m_piData2 = new int32_t [m_iSize];
   m_piNext = new int [m_iSize];

   
   for (int i = 0; i < size; ++ i)
   {
      m_piData1[i] = -1;
      m_piData2[i] = -1;
   }

   
   #ifndef WINDOWS
      pthread_mutex_init(&m_ListLock, 0);
   #else
      m_ListLock = CreateMutex(NULL, false, NULL);
   #endif
}

CSndLossList::~CSndLossList()
{
   delete [] m_piData1;
   delete [] m_piData2;
   delete [] m_piNext;

   #ifndef WINDOWS
      pthread_mutex_destroy(&m_ListLock);
   #else
      CloseHandle(m_ListLock);
   #endif
}

int CSndLossList::insert(int32_t seqno1, int32_t seqno2)
{
   CGuard listguard(m_ListLock);

   if (0 == m_iLength)
   {
      

      m_iHead = 0;
      m_piData1[m_iHead] = seqno1;
      if (seqno2 != seqno1)
         m_piData2[m_iHead] = seqno2;

      m_piNext[m_iHead] = -1;
      m_iLastInsertPos = m_iHead;

      m_iLength += CSeqNo::seqlen(seqno1, seqno2);

      return m_iLength;
   }

   
   int origlen = m_iLength;
   int offset = CSeqNo::seqoff(m_piData1[m_iHead], seqno1);
   int loc = (m_iHead + offset + m_iSize) % m_iSize;

   if (offset < 0)
   {
      

      m_piData1[loc] = seqno1;
      if (seqno2 != seqno1)
         m_piData2[loc] = seqno2;

      
      m_piNext[loc] = m_iHead;
      m_iHead = loc;
      m_iLastInsertPos = loc;

      m_iLength += CSeqNo::seqlen(seqno1, seqno2);
   }
   else if (offset > 0)
   {
      if (seqno1 == m_piData1[loc])
      {
         m_iLastInsertPos = loc;

         
         if (-1 == m_piData2[loc])
         {
            if (seqno2 != seqno1)
            {
               m_iLength += CSeqNo::seqlen(seqno1, seqno2) - 1;
               m_piData2[loc] = seqno2;
            }
         }
         else if (CSeqNo::seqcmp(seqno2, m_piData2[loc]) > 0)
         {
            
            m_iLength += CSeqNo::seqlen(m_piData2[loc], seqno2) - 1;
            m_piData2[loc] = seqno2;
         }
         else
            
            return 0;
      }
      else
      {
         
         int i;
         if ((-1 != m_iLastInsertPos) && (CSeqNo::seqcmp(m_piData1[m_iLastInsertPos], seqno1) < 0))
            i = m_iLastInsertPos;
         else
            i = m_iHead;

         while ((-1 != m_piNext[i]) && (CSeqNo::seqcmp(m_piData1[m_piNext[i]], seqno1) < 0))
            i = m_piNext[i];

         if ((-1 == m_piData2[i]) || (CSeqNo::seqcmp(m_piData2[i], seqno1) < 0))
         {
            m_iLastInsertPos = loc;

            
            m_piData1[loc] = seqno1;
            if (seqno2 != seqno1)
               m_piData2[loc] = seqno2;

            m_piNext[loc] = m_piNext[i];
            m_piNext[i] = loc;

            m_iLength += CSeqNo::seqlen(seqno1, seqno2);
         }
         else
         {
            m_iLastInsertPos = i;

            
            if (CSeqNo::seqcmp(m_piData2[i], seqno2) < 0)
            {
               m_iLength += CSeqNo::seqlen(m_piData2[i], seqno2) - 1;
               m_piData2[i] = seqno2;

               loc = i;
            }
            else
               return 0;
         }
      }
   }
   else
   {
      m_iLastInsertPos = m_iHead;

      
      if (seqno2 != seqno1)
      {
         if (-1 == m_piData2[loc])
         {
            m_iLength += CSeqNo::seqlen(seqno1, seqno2) - 1;
            m_piData2[loc] = seqno2;
         }
         else if (CSeqNo::seqcmp(seqno2, m_piData2[loc]) > 0)
         {
            m_iLength += CSeqNo::seqlen(m_piData2[loc], seqno2) - 1;
            m_piData2[loc] = seqno2;
         }
         else
            return 0;
      }
      else
         return 0;
   }

   
   while ((-1 != m_piNext[loc]) && (-1 != m_piData2[loc]))
   {
      int i = m_piNext[loc];

      if (CSeqNo::seqcmp(m_piData1[i], CSeqNo::incseq(m_piData2[loc])) <= 0)
      {
         
         if (-1 != m_piData2[i])
         {
            if (CSeqNo::seqcmp(m_piData2[i], m_piData2[loc]) > 0)
            {
               if (CSeqNo::seqcmp(m_piData2[loc], m_piData1[i]) >= 0)
                  m_iLength -= CSeqNo::seqlen(m_piData1[i], m_piData2[loc]);

               m_piData2[loc] = m_piData2[i];
            }
            else
               m_iLength -= CSeqNo::seqlen(m_piData1[i], m_piData2[i]);
         }
         else
         {
            if (m_piData1[i] == CSeqNo::incseq(m_piData2[loc]))
               m_piData2[loc] = m_piData1[i];
            else
               m_iLength --;
         }

         m_piData1[i] = -1;
         m_piData2[i] = -1;
         m_piNext[loc] = m_piNext[i];
      }
      else
         break;
   }

   return m_iLength - origlen;
}

void CSndLossList::remove(int32_t seqno)
{
   CGuard listguard(m_ListLock);

   if (0 == m_iLength)
      return;

   
   int offset = CSeqNo::seqoff(m_piData1[m_iHead], seqno);
   int loc = (m_iHead + offset + m_iSize) % m_iSize;

   if (0 == offset)
   {
      
      loc = (loc + 1) % m_iSize;

      if (-1 == m_piData2[m_iHead])
         loc = m_piNext[m_iHead];
      else
      {
         m_piData1[loc] = CSeqNo::incseq(seqno);
         if (CSeqNo::seqcmp(m_piData2[m_iHead], CSeqNo::incseq(seqno)) > 0)
            m_piData2[loc] = m_piData2[m_iHead];

         m_piData2[m_iHead] = -1;

         m_piNext[loc] = m_piNext[m_iHead];
      }

      m_piData1[m_iHead] = -1;

      if (m_iLastInsertPos == m_iHead)
         m_iLastInsertPos = -1;

      m_iHead = loc;

      m_iLength --;
   }
   else if (offset > 0)
   {
      int h = m_iHead;

      if (seqno == m_piData1[loc])
      {
         
         int temp = loc;
         loc = (loc + 1) % m_iSize;

         if (-1 == m_piData2[temp])
            m_iHead = m_piNext[temp];
         else
         {
            
            m_piData1[loc] = CSeqNo::incseq(seqno);
            if (CSeqNo::seqcmp(m_piData2[temp], m_piData1[loc]) > 0)
               m_piData2[loc] = m_piData2[temp];
            m_iHead = loc;
            m_piNext[loc] = m_piNext[temp];
            m_piNext[temp] = loc;
            m_piData2[temp] = -1;
         }
      }
      else
      {
         
         int i = m_iHead;
         while ((-1 != m_piNext[i]) && (CSeqNo::seqcmp(m_piData1[m_piNext[i]], seqno) < 0))
            i = m_piNext[i];

         loc = (loc + 1) % m_iSize;

         if (-1 == m_piData2[i])
            m_iHead = m_piNext[i];
         else if (CSeqNo::seqcmp(m_piData2[i], seqno) > 0)
         {
            
            m_piData1[loc] = CSeqNo::incseq(seqno);
            if (CSeqNo::seqcmp(m_piData2[i], m_piData1[loc]) > 0)
               m_piData2[loc] = m_piData2[i];

            m_piData2[i] = seqno;

            m_piNext[loc] = m_piNext[i];
            m_piNext[i] = loc;

            m_iHead = loc;
         }
         else
            m_iHead = m_piNext[i];
      }

      
      while (h != m_iHead)
      {
         if (m_piData2[h] != -1)
         {
            m_iLength -= CSeqNo::seqlen(m_piData1[h], m_piData2[h]);
            m_piData2[h] = -1;
         }
         else
            m_iLength --;

         m_piData1[h] = -1;

         if (m_iLastInsertPos == h)
            m_iLastInsertPos = -1;

         h = m_piNext[h];
      }
   }
}

int CSndLossList::getLossLength()
{
   CGuard listguard(m_ListLock);

   return m_iLength;
}

int32_t CSndLossList::getLostSeq()
{
   if (0 == m_iLength)
     return -1;

   CGuard listguard(m_ListLock);

   if (0 == m_iLength)
     return -1;

   if (m_iLastInsertPos == m_iHead)
      m_iLastInsertPos = -1;

   
   int32_t seqno = m_piData1[m_iHead];

   
   if (-1 == m_piData2[m_iHead])
   {
      
      m_piData1[m_iHead] = -1;
      m_iHead = m_piNext[m_iHead];
   }
   else
   {
      
      int loc = (m_iHead + 1) % m_iSize;

      m_piData1[loc] = CSeqNo::incseq(seqno);
      if (CSeqNo::seqcmp(m_piData2[m_iHead], m_piData1[loc]) > 0)
         m_piData2[loc] = m_piData2[m_iHead];

      m_piData1[m_iHead] = -1;
      m_piData2[m_iHead] = -1;

      m_piNext[loc] = m_piNext[m_iHead];
      m_iHead = loc;
   }

   m_iLength --;

   return seqno;
}



CRcvLossList::CRcvLossList(int size):
m_piData1(NULL),
m_piData2(NULL),
m_piNext(NULL),
m_piPrior(NULL),
m_iHead(-1),
m_iTail(-1),
m_iLength(0),
m_iSize(size)
{
   m_piData1 = new int32_t [m_iSize];
   m_piData2 = new int32_t [m_iSize];
   m_piNext = new int [m_iSize];
   m_piPrior = new int [m_iSize];

   
   for (int i = 0; i < size; ++ i)
   {
      m_piData1[i] = -1;
      m_piData2[i] = -1;
   }
}

CRcvLossList::~CRcvLossList()
{
   delete [] m_piData1;
   delete [] m_piData2;
   delete [] m_piNext;
   delete [] m_piPrior;
}

void CRcvLossList::insert(int32_t seqno1, int32_t seqno2)
{
   
   

   if (0 == m_iLength)
   {
      
      m_iHead = 0;
      m_iTail = 0;
      m_piData1[m_iHead] = seqno1;
      if (seqno2 != seqno1)
         m_piData2[m_iHead] = seqno2;

      m_piNext[m_iHead] = -1;
      m_piPrior[m_iHead] = -1;
      m_iLength += CSeqNo::seqlen(seqno1, seqno2);

      return;
   }

   
   int offset = CSeqNo::seqoff(m_piData1[m_iHead], seqno1);
   int loc = (m_iHead + offset) % m_iSize;

   if ((-1 != m_piData2[m_iTail]) && (CSeqNo::incseq(m_piData2[m_iTail]) == seqno1))
   {
      
      loc = m_iTail;
      m_piData2[loc] = seqno2;
   }
   else
   {
      
      m_piData1[loc] = seqno1;

      if (seqno2 != seqno1)
         m_piData2[loc] = seqno2;

      m_piNext[m_iTail] = loc;
      m_piPrior[loc] = m_iTail;
      m_piNext[loc] = -1;
      m_iTail = loc;
   }

   m_iLength += CSeqNo::seqlen(seqno1, seqno2);
}

bool CRcvLossList::remove(int32_t seqno)
{
   if (0 == m_iLength)
      return false;

   
   int offset = CSeqNo::seqoff(m_piData1[m_iHead], seqno);
   if (offset < 0)
      return false;

   int loc = (m_iHead + offset) % m_iSize;

   if (seqno == m_piData1[loc])
   {
      

      if (-1 == m_piData2[loc])
      {
         
         if (m_iHead == loc)
         {
            m_iHead = m_piNext[m_iHead];
            if (-1 != m_iHead)
               m_piPrior[m_iHead] = -1;
         }
         else
         {
            m_piNext[m_piPrior[loc]] = m_piNext[loc];
            if (-1 != m_piNext[loc])
               m_piPrior[m_piNext[loc]] = m_piPrior[loc];
            else
               m_iTail = m_piPrior[loc];
         }

         m_piData1[loc] = -1;
      }
      else
      {
         
         

         
         int i = (loc + 1) % m_iSize;

         
         m_piData1[i] = CSeqNo::incseq(m_piData1[loc]);

         
         if (CSeqNo::seqcmp(m_piData2[loc], CSeqNo::incseq(m_piData1[loc])) > 0)
            m_piData2[i] = m_piData2[loc];

         
         m_piData1[loc] = -1;
         m_piData2[loc] = -1;

         
         m_piNext[i] = m_piNext[loc];
         m_piPrior[i] = m_piPrior[loc];

         if (m_iHead == loc)
            m_iHead = i;
         else
            m_piNext[m_piPrior[i]] = i;

         if (m_iTail == loc)
            m_iTail = i;
         else
            m_piPrior[m_piNext[i]] = i;
      }

      m_iLength --;

      return true;
   }

   
   

   
   int i = (loc - 1 + m_iSize) % m_iSize;
   while (-1 == m_piData1[i])
      i = (i - 1 + m_iSize) % m_iSize;

   
   if ((-1 == m_piData2[i]) || (CSeqNo::seqcmp(seqno, m_piData2[i]) > 0))
       return false;

   if (seqno == m_piData2[i])
   {
      

      if (seqno == CSeqNo::incseq(m_piData1[i]))
         m_piData2[i] = -1;
      else
         m_piData2[i] = CSeqNo::decseq(seqno);
   }
   else
   {
      

      
      
      loc = (loc + 1) % m_iSize;

      m_piData1[loc] = CSeqNo::incseq(seqno);
      if (CSeqNo::seqcmp(m_piData2[i], m_piData1[loc]) > 0)
         m_piData2[loc] = m_piData2[i];

      
      if (seqno == CSeqNo::incseq(m_piData1[i]))
         m_piData2[i] = -1;
      else
         m_piData2[i] = CSeqNo::decseq(seqno);

      
      m_piNext[loc] = m_piNext[i];
      m_piNext[i] = loc;
      m_piPrior[loc] = i;

      if (m_iTail == i)
         m_iTail = loc;
      else
         m_piPrior[m_piNext[loc]] = loc;
   }

   m_iLength --;

   return true;
}

bool CRcvLossList::remove(int32_t seqno1, int32_t seqno2)
{
   if (seqno1 <= seqno2)
   {
      for (int32_t i = seqno1; i <= seqno2; ++ i)
         remove(i);
   }
   else
   {
      for (int32_t j = seqno1; j < CSeqNo::m_iMaxSeqNo; ++ j)
         remove(j);
      for (int32_t k = 0; k <= seqno2; ++ k)
         remove(k);
   }

   return true;
}

bool CRcvLossList::find(int32_t seqno1, int32_t seqno2) const
{
   if (0 == m_iLength)
      return false;

   int p = m_iHead;

   while (-1 != p)
   {
      if ((CSeqNo::seqcmp(m_piData1[p], seqno1) == 0) ||
          ((CSeqNo::seqcmp(m_piData1[p], seqno1) > 0) && (CSeqNo::seqcmp(m_piData1[p], seqno2) <= 0)) ||
          ((CSeqNo::seqcmp(m_piData1[p], seqno1) < 0) && (m_piData2[p] != -1) && CSeqNo::seqcmp(m_piData2[p], seqno1) >= 0))
          return true;

      p = m_piNext[p];
   }

   return false;
}

int CRcvLossList::getLossLength() const
{
   return m_iLength;
}

int CRcvLossList::getFirstLostSeq() const
{
   if (0 == m_iLength)
      return -1;

   return m_piData1[m_iHead];
}

void CRcvLossList::getLossArray(int32_t* array, int& len, int limit)
{
   len = 0;

   int i = m_iHead;

   while ((len < limit - 1) && (-1 != i))
   {
      array[len] = m_piData1[i];
      if (-1 != m_piData2[i])
      {
         
         array[len] |= 0x80000000;
         ++ len;
         array[len] = m_piData2[i];
      }

      ++ len;

      i = m_piNext[i];
   }
}
