



#ifndef __UDT_LIST_H__
#define __UDT_LIST_H__


#include "udt.h"
#include "udtCommon.h"


class CSndLossList
{
public:
   CSndLossList(int size = 1024);
   ~CSndLossList();

      
      
      
      
      
      
      

   int insert(int32_t seqno1, int32_t seqno2);

      
      
      
      
      
      

   void remove(int32_t seqno);

      
      
      
      
      
      

   int getLossLength();

      
      
      
      
      
      

   int32_t getLostSeq();

private:
   int32_t* m_piData1;                  
   int32_t* m_piData2;                  
   int* m_piNext;                       

   int m_iHead;                         
   int m_iLength;                       
   int m_iSize;                         
   int m_iLastInsertPos;                

   udt_pthread_mutex_t m_ListLock;          

private:
   CSndLossList(const CSndLossList&);
   CSndLossList& operator=(const CSndLossList&);
};



class CRcvLossList
{
public:
   CRcvLossList(int size = 1024);
   ~CRcvLossList();

      
      
      
      
      
      
      

   void insert(int32_t seqno1, int32_t seqno2);

      
      
      
      
      
      

   bool remove(int32_t seqno);

      
      
      
      
      
      
      

   bool remove(int32_t seqno1, int32_t seqno2);

      
      
      
      
      
      
      

   bool find(int32_t seqno1, int32_t seqno2) const;

      
      
      
      
      
      

   int getLossLength() const;

      
      
      
      
      
      

   int getFirstLostSeq() const;

      
      
      
      
      
      
      
      

   void getLossArray(int32_t* array, int& len, int limit);

private:
   int32_t* m_piData1;                  
   int32_t* m_piData2;                  
   int* m_piNext;                       
   int* m_piPrior;                      

   int m_iHead;                         
   int m_iTail;                         
   int m_iLength;                       
   int m_iSize;                         

private:
   CRcvLossList(const CRcvLossList&);
   CRcvLossList& operator=(const CRcvLossList&);
};


#endif
