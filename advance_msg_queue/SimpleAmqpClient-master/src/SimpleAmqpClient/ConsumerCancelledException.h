#ifndef SIMPLEAMQPCLIENT_CONSUMERCANCELLEDEXCEPTION_H
#define SIMPLEAMQPCLIENT_CONSUMERCANCELLEDEXCEPTION_H


#include <stdexcept>
#include <string>

#include "SimpleAmqpClient/Util.h"




namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT ConsumerCancelledException
    : public std::runtime_error {
 public:
  
  explicit ConsumerCancelledException(const std::string &consumer_tag) throw()
      : std::runtime_error(
            std::string("Consumer was cancelled: ").append(consumer_tag)),
        m_consumer_tag(consumer_tag) {}

  
  virtual ~ConsumerCancelledException() throw() {}

  
  std::string GetConsumerTag() const { return m_consumer_tag; }

 private:
  std::string m_consumer_tag;
};
}  
#endif  
