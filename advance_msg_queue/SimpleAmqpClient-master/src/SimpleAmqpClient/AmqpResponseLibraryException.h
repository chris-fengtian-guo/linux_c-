#ifndef SIMPLEAMQPCLIENT_AMQPRESPONSELIBRARYEXCEPTION_H
#define SIMPLEAMQPCLIENT_AMQPRESPONSELIBRARYEXCEPTION_H


#include <boost/cstdint.hpp>
#include <stdexcept>
#include <string>

#include "SimpleAmqpClient/Util.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251 4275)
#endif




struct amqp_rpc_reply_t_;

namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT AmqpResponseLibraryException
    : public std::runtime_error {
 public:
  
  static AmqpResponseLibraryException CreateException(
      const amqp_rpc_reply_t_ &reply, const std::string &context);

 protected:
  
  explicit AmqpResponseLibraryException(const std::string &message) throw();
};

}  

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  
