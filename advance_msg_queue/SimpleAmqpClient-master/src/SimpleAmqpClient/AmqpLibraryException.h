#ifndef SIMPLEAMQPCLIENT_AMQPLIBRARYEXCEPTION_H
#define SIMPLEAMQPCLIENT_AMQPLIBRARYEXCEPTION_H


#include <stdexcept>
#include <string>

#include "SimpleAmqpClient/Util.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251 4275)
#endif




struct amqp_rpc_reply_t_;

namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT AmqpLibraryException : public std::runtime_error {
 public:
  
  static AmqpLibraryException CreateException(int error_code);
  
  static AmqpLibraryException CreateException(int error_code,
                                              const std::string &context);

  
  int ErrorCode() const { return m_errorCode; }

 protected:
  
  explicit AmqpLibraryException(const std::string &message,
                                int error_code) throw();
  

 private:
  int m_errorCode;
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  
