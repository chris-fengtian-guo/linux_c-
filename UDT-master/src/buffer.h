



#ifndef __UDT_BUFFER_H__
#define __UDT_BUFFER_H__


#include "udt.h"
#include "list.h"
#include "queue.h"
#include <fstream>

class CSndBuffer
{
public:
   CSndBuffer(int size = 32, int mss = 1500);
   ~CSndBuffer();

      
      
      
      
      
      
      
      
      

   void addBuffer(const char* data, int len, int ttl = -1, bool order = false);

      
      
      
      
      
      
      

   int addBufferFromFile(std::fstream& ifs, int len);

      
      
      
      
      
      
      

   int readData(char** data, int32_t& msgno);

      
      
      
      
      
      
      
      
      

   int readData(char** data, const int offset, int32_t& msgno, int& msglen);

      
      
      
      
      
      

   void ackData(int offset);

      
      
      
      
      
      

   int getCurrBufSize() const;

private:
   void increase();

private:
   udt_pthread_mutex_t m_BufLock;           

   struct Block
   {
      char* m_pcData;                   
      int m_iLength;                    

      int32_t m_iMsgNo;                 
      uint64_t m_OriginTime;            
      int m_iTTL;                       

      Block* m_pNext;                   
   } *m_pBlock, *m_pFirstBlock, *m_pCurrBlock, *m_pLastBlock;

   
   
   
   

   struct Buffer
   {
      char* m_pcData;			
      int m_iSize;			
      Buffer* m_pNext;			
   } *m_pBuffer;			

   int32_t m_iNextMsgNo;                

   int m_iSize;				
   int m_iMSS;                          

   int m_iCount;			

private:
   CSndBuffer(const CSndBuffer&);
   CSndBuffer& operator=(const CSndBuffer&);
};



class CRcvBuffer
{
public:
   CRcvBuffer(CUnitQueue* queue, int bufsize = 65536);
   ~CRcvBuffer();

      
      
      
      
      
      
      

   int addData(CUnit* unit, int offset);

      
      
      
      
      
      
      

   int readBuffer(char* data, int len);

      
      
      
      
      
      
      

   int readBufferToFile(std::fstream& ofs, int len);

      
      
      
      
      
      

   void ackData(int len);

      
      
      
      
      
      

   int getAvailBufSize() const;

      
      
      
      
      
      

   int getRcvDataSize() const;

      
      
      
      
      
      

   void dropMsg(int32_t msgno);

      
      
      
      
      
      
      

   int readMsg(char* data, int len);

      
      
      
      
      
      

   int getRcvMsgNum();

private:
   bool scanMsg(int& start, int& end, bool& passack);

private:
   CUnit** m_pUnit;                     
   int m_iSize;                         
   CUnitQueue* m_pUnitQueue;		

   int m_iStartPos;                     
   int m_iLastAckPos;                   
                    
   int m_iMaxPos;			

   int m_iNotch;			

private:
   CRcvBuffer();
   CRcvBuffer(const CRcvBuffer&);
   CRcvBuffer& operator=(const CRcvBuffer&);
};


#endif
