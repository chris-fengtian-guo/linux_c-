#ifndef SIMPLEAMQPCLIENT_BASICMESSAGE_H
#define SIMPLEAMQPCLIENT_BASICMESSAGE_H


#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <string>

#include "SimpleAmqpClient/Table.h"
#include "SimpleAmqpClient/Util.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4275 4251)
#endif




namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT BasicMessage : boost::noncopyable {
 public:
  
  typedef boost::shared_ptr<BasicMessage> ptr_t;

  
  enum delivery_mode_t {
    dm_notset = 0,
    dm_nonpersistent = 1,
    dm_persistent = 2
  };

  
  static ptr_t Create() { return boost::make_shared<BasicMessage>(); }

  
  static ptr_t Create(const std::string& body) {
    return boost::make_shared<BasicMessage>(body);
  }

  
  BasicMessage();
  
  BasicMessage(const std::string& body);

 public:
  
  virtual ~BasicMessage();

  
  const std::string& Body() const;
  std::string& Body();

  
  void Body(const std::string& body);

  
  const std::string& ContentType() const;
  
  void ContentType(const std::string& content_type);
  
  bool ContentTypeIsSet() const;
  
  void ContentTypeClear();

  
  const std::string& ContentEncoding() const;
  
  void ContentEncoding(const std::string& content_encoding);
  
  bool ContentEncodingIsSet() const;
  
  void ContentEncodingClear();

  
  delivery_mode_t DeliveryMode() const;
  
  void DeliveryMode(delivery_mode_t delivery_mode);
  
  bool DeliveryModeIsSet() const;
  
  void DeliveryModeClear();

  
  boost::uint8_t Priority() const;
  
  void Priority(boost::uint8_t priority);
  
  bool PriorityIsSet() const;
  
  void PriorityClear();

  
  const std::string& CorrelationId() const;
  
  void CorrelationId(const std::string& correlation_id);
  
  bool CorrelationIdIsSet() const;
  
  void CorrelationIdClear();

  
  const std::string& ReplyTo() const;
  
  void ReplyTo(const std::string& reply_to);
  
  bool ReplyToIsSet() const;
  
  void ReplyToClear();

  
  const std::string& Expiration() const;
  
  void Expiration(const std::string& expiration);
  
  bool ExpirationIsSet() const;
  
  void ExpirationClear();

  
  const std::string& MessageId() const;
  
  void MessageId(const std::string& message_id);
  
  bool MessageIdIsSet() const;
  
  void MessageIdClear();

  
  boost::uint64_t Timestamp() const;
  
  void Timestamp(boost::uint64_t timestamp);
  
  bool TimestampIsSet() const;
  
  void TimestampClear();

  
  const std::string& Type() const;
  
  void Type(const std::string& type);
  
  bool TypeIsSet() const;
  
  void TypeClear();

  
  const std::string& UserId() const;
  
  void UserId(const std::string& user_id);
  
  bool UserIdIsSet() const;
  
  void UserIdClear();

  
  const std::string& AppId() const;
  
  void AppId(const std::string& app_id);
  
  bool AppIdIsSet() const;
  
  void AppIdClear();

  
  const std::string& ClusterId() const;
  
  void ClusterId(const std::string& cluster_id);
  
  bool ClusterIdIsSet() const;
  
  void ClusterIdClear();

  
  Table& HeaderTable();
  const Table& HeaderTable() const;
  
  void HeaderTable(const Table& header_table);
  
  bool HeaderTableIsSet() const;
  
  void HeaderTableClear();

 protected:
  struct Impl;
  
  boost::scoped_ptr<Impl> m_impl;
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  
