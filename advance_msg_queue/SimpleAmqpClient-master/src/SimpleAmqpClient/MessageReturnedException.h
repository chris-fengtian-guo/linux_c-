#ifndef SIMPLEAMQPCLIENT_MESSAGERETURNEDEXCEPTION_H
#define SIMPLEAMQPCLIENT_MESSAGERETURNEDEXCEPTION_H


#include <boost/cstdint.hpp>
#include <stdexcept>

#include "SimpleAmqpClient/BasicMessage.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251 4275)
#endif




namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT MessageReturnedException
    : public std::runtime_error {
 public:
  
  explicit MessageReturnedException(BasicMessage::ptr_t message,
                                    boost::uint32_t reply_code,
                                    const std::string &reply_text,
                                    const std::string &exchange,
                                    const std::string &routing_key) throw();

  virtual ~MessageReturnedException() throw() {}

  
  BasicMessage::ptr_t message() const throw() { return m_message; }
  
  boost::uint32_t reply_code() const throw() { return m_reply_code; }
  
  std::string reply_text() const throw() { return m_reply_text; }
  
  std::string exchange() const throw() { return m_exchange; }
  
  std::string routing_key() const throw() { return m_routing_key; }

 private:
  BasicMessage::ptr_t m_message;
  boost::uint32_t m_reply_code;
  std::string m_reply_text;
  std::string m_exchange;
  std::string m_routing_key;
  mutable std::string m_what;
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  
