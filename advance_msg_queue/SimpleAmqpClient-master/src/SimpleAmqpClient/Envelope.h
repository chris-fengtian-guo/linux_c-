#ifndef SIMPLEAMQPCLIENT_ENVELOPE_H
#define SIMPLEAMQPCLIENT_ENVELOPE_H


#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "SimpleAmqpClient/BasicMessage.h"
#include "SimpleAmqpClient/Util.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4275 4251)
#endif  




namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT Envelope : boost::noncopyable {
 public:
  
  typedef boost::shared_ptr<Envelope> ptr_t;

  
  static ptr_t Create(const BasicMessage::ptr_t message,
                      const std::string &consumer_tag,
                      const boost::uint64_t delivery_tag,
                      const std::string &exchange, bool redelivered,
                      const std::string &routing_key,
                      const boost::uint16_t delivery_channel) {
    return boost::make_shared<Envelope>(message, consumer_tag, delivery_tag,
                                        exchange, redelivered, routing_key,
                                        delivery_channel);
  }

  
  explicit Envelope(const BasicMessage::ptr_t message,
                    const std::string &consumer_tag,
                    const boost::uint64_t delivery_tag,
                    const std::string &exchange, bool redelivered,
                    const std::string &routing_key,
                    const boost::uint16_t delivery_channel);

 public:
  
  virtual ~Envelope();

  
  inline BasicMessage::ptr_t Message() const { return m_message; }

  
  inline std::string ConsumerTag() const { return m_consumerTag; }

  
  inline boost::uint64_t DeliveryTag() const { return m_deliveryTag; }

  
  inline std::string Exchange() const { return m_exchange; }

  
  inline bool Redelivered() const { return m_redelivered; }

  
  inline std::string RoutingKey() const { return m_routingKey; }

  
  inline boost::uint16_t DeliveryChannel() const { return m_deliveryChannel; }

  
  struct DeliveryInfo {
    
    
    boost::uint64_t delivery_tag;
    
    boost::uint16_t delivery_channel;
  };

  
  inline DeliveryInfo GetDeliveryInfo() const {
    DeliveryInfo info;
    info.delivery_tag = m_deliveryTag;
    info.delivery_channel = m_deliveryChannel;

    return info;
  }

 private:
  const BasicMessage::ptr_t m_message;
  const std::string m_consumerTag;
  const boost::uint64_t m_deliveryTag;
  const std::string m_exchange;
  const bool m_redelivered;
  const std::string m_routingKey;
  const boost::uint16_t m_deliveryChannel;
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif  

#endif  
