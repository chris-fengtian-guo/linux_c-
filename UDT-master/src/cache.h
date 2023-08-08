



#ifndef __UDT_CACHE_H__
#define __UDT_CACHE_H__

#include <list>
#include <vector>

#include "udtCommon.h"
#include "udt.h"

class CCacheItem
{
public:
   virtual ~CCacheItem() {}

public:
   virtual CCacheItem& operator=(const CCacheItem&) = 0;

   
   virtual bool operator==(const CCacheItem&) = 0;

      
      
      
      
      
      

   virtual CCacheItem* clone() = 0;

      
      
      
      
      
      

   virtual int getKey() = 0;

   
   
   virtual void release() {}
};

template<typename T> class CCache
{
public:
   CCache(int size = 1024):
   m_iMaxSize(size),
   m_iHashSize(size * 3),
   m_iCurrSize(0)
   {
      m_vHashPtr.resize(m_iHashSize);
      CGuard::createMutex(m_Lock);
   }

   ~CCache()
   {
      clear();
      CGuard::releaseMutex(m_Lock);
   }

public:
      
      
      
      
      
      

   int lookup(T* data)
   {
      CGuard cacheguard(m_Lock);

      int key = data->getKey();
      if (key < 0)
         return -1;
      if (key >= m_iMaxSize)
         key %= m_iHashSize;

      const ItemPtrList& item_list = m_vHashPtr[key];
      for (typename ItemPtrList::const_iterator i = item_list.begin(); i != item_list.end(); ++ i)
      {
         if (*data == ***i)
         {
            
            *data = ***i;
            return 0;
         }
      }

      return -1;
   }

      
      
      
      
      
      

   int update(T* data)
   {
      CGuard cacheguard(m_Lock);

      int key = data->getKey();
      if (key < 0)
         return -1;
      if (key >= m_iMaxSize)
         key %= m_iHashSize;

      T* curr = NULL;

      ItemPtrList& item_list = m_vHashPtr[key];
      for (typename ItemPtrList::iterator i = item_list.begin(); i != item_list.end(); ++ i)
      {
         if (*data == ***i)
         {
            
            ***i = *data;
            curr = **i;

            
            m_StorageList.erase(*i);
            item_list.erase(i);

            
            m_StorageList.push_front(curr);
            item_list.push_front(m_StorageList.begin());

            return 0;
         }
      }

      
      curr = data->clone();
      m_StorageList.push_front(curr);
      item_list.push_front(m_StorageList.begin());

      ++ m_iCurrSize;
      if (m_iCurrSize >= m_iMaxSize)
      {
         
         T* last_data = m_StorageList.back();
         int last_key = last_data->getKey() % m_iHashSize;

         item_list = m_vHashPtr[last_key];
         for (typename ItemPtrList::iterator i = item_list.begin(); i != item_list.end(); ++ i)
         {
            if (*last_data == ***i)
            {
               item_list.erase(i);
               break;
            }
         }

         last_data->release();
         delete last_data;
         m_StorageList.pop_back();
         -- m_iCurrSize;
      }

      return 0;
   }

      
      
      
      
      
      

   void setSizeLimit(int size)
   {
      m_iMaxSize = size;
      m_iHashSize = size * 3;
      m_vHashPtr.resize(m_iHashSize);
   }

      
      
      
      
      
      

   void clear()
   {
      for (typename std::list<T*>::iterator i = m_StorageList.begin(); i != m_StorageList.end(); ++ i)
      {
         (*i)->release();
         delete *i;
      }
      m_StorageList.clear();
      for (typename std::vector<ItemPtrList>::iterator i = m_vHashPtr.begin(); i != m_vHashPtr.end(); ++ i)
         i->clear();
      m_iCurrSize = 0;
   }

private:
   std::list<T*> m_StorageList;
   typedef typename std::list<T*>::iterator ItemPtr;
   typedef std::list<ItemPtr> ItemPtrList;
   std::vector<ItemPtrList> m_vHashPtr;

   int m_iMaxSize;
   int m_iHashSize;
   int m_iCurrSize;

   udt_pthread_mutex_t m_Lock;

private:
   CCache(const CCache&);
   CCache& operator=(const CCache&);
};


class CInfoBlock
{
public:
   uint32_t m_piIP[4];		
   int m_iIPversion;		
   uint64_t m_ullTimeStamp;	
   int m_iRTT;			
   int m_iBandwidth;		
   int m_iLossRate;		
   int m_iReorderDistance;	
   double m_dInterval;		
   double m_dCWnd;		

public:
   virtual ~CInfoBlock() {}
   virtual CInfoBlock& operator=(const CInfoBlock& obj);
   virtual bool operator==(const CInfoBlock& obj);
   virtual CInfoBlock* clone();
   virtual int getKey();
   virtual void release() {}

public:

      
      
      
      
      
      
      
      

   static void convert(const sockaddr* addr, int ver, uint32_t ip[]);
};


#endif
