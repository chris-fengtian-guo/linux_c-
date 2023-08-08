#ifndef SIMPLEAMQPCLIENT_MESSAGEREJECTEDEXCEPTION_H
#define SIMPLEAMQPCLIENT_MESSAGEREJECTEDEXCEPTION_H


#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>

#include "SimpleAmqpClient/BasicMessage.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251 4275)
#endif




namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT MessageRejectedException
    : public std::runtime_error {
 public:
  MessageRejectedException(uint64_t delivery_tag)
      : std::runtime_error(
            std::string("Message rejected: ")
                .append(boost::lexical_cast<std::string>(delivery_tag))),
        m_delivery_tag(delivery_tag) {}

  
  uint64_t GetDeliveryTag() { return m_delivery_tag; }

 private:
  uint64_t m_delivery_tag;
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  
