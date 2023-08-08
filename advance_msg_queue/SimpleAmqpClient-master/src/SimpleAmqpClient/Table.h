#ifndef SIMPLEAMQPCLIENT_TABLE_H
#define SIMPLEAMQPCLIENT_TABLE_H


#include <boost/cstdint.hpp>
#include <boost/scoped_ptr.hpp>
#include <ctime>
#include <map>
#include <string>
#include <vector>

#include "SimpleAmqpClient/Util.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#endif




namespace AmqpClient {

namespace Detail {
class TableValueImpl;
}  


typedef std::string TableKey;

class TableValue;


typedef std::vector<TableValue> Array;


typedef std::map<TableKey, TableValue> Table;

typedef Table::value_type TableEntry;


class SIMPLEAMQPCLIENT_EXPORT TableValue {
 public:
  friend class Detail::TableValueImpl;

  
  enum ValueType {
    VT_void = 0,        
    VT_bool = 1,        
    VT_int8 = 2,        
    VT_int16 = 3,       
    VT_int32 = 4,       
    VT_int64 = 5,       
    VT_float = 6,       
    VT_double = 7,      
    VT_string = 8,      
    VT_array = 9,       
    VT_table = 10,      
    VT_uint8 = 11,      
    VT_uint16 = 12,     
    VT_uint32 = 13,     
    VT_timestamp = 14,  
  };

  
  TableValue();

  
  TableValue(bool value);

  
  TableValue(boost::uint8_t value);

  
  TableValue(boost::int8_t value);

  
  TableValue(boost::uint16_t value);

  
  TableValue(boost::int16_t value);

  
  TableValue(boost::uint32_t value);

  
  TableValue(boost::int32_t value);

 private:
  
  TableValue(boost::uint64_t value);

 public:
  
  static TableValue Timestamp(std::time_t value);

  
  TableValue(boost::int64_t value);

  
  TableValue(float value);

  
  TableValue(double value);

  
  TableValue(const char *value);

  
  TableValue(const std::string &value);

  
  TableValue(const std::vector<TableValue> &values);

  
  TableValue(const Table &value);

  
  TableValue(const TableValue &l);

  
  TableValue &operator=(const TableValue &l);

  
  bool operator==(const TableValue &l) const;

  
  bool operator!=(const TableValue &l) const;

  
  virtual ~TableValue();

  
  ValueType GetType() const;

  
  bool GetBool() const;

  
  boost::uint8_t GetUint8() const;

  
  boost::int8_t GetInt8() const;

  
  boost::uint16_t GetUint16() const;

  
  boost::int16_t GetInt16() const;

  
  boost::uint32_t GetUint32() const;

  
  boost::int32_t GetInt32() const;

  
  boost::uint64_t GetUint64() const;

  
  std::time_t GetTimestamp() const;

  
  boost::int64_t GetInt64() const;

  
  boost::int64_t GetInteger() const;

  
  float GetFloat() const;

  
  double GetDouble() const;

  
  double GetReal() const;

  
  std::string GetString() const;

  
  std::vector<TableValue> GetArray() const;

  
  Table GetTable() const;

  
  void Set();

  
  void Set(bool value);

  
  void Set(boost::uint8_t value);

  
  void Set(boost::int8_t value);

  
  void Set(boost::uint16_t value);

  
  void Set(boost::int16_t value);

  
  void Set(boost::uint32_t value);

  
  void Set(boost::int32_t value);

  
  void SetTimestamp(std::time_t value);

  
  void Set(boost::int64_t value);

  
  void Set(float value);

  
  void Set(double value);

  
  void Set(const char *value);

  
  void Set(const std::string &value);

  
  void Set(const std::vector<TableValue> &value);

  
  void Set(const Table &value);

 private:
  boost::scoped_ptr<Detail::TableValueImpl> m_impl;
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  
