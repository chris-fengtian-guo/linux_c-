


#include "SimpleAmqpClient/AmqpLibraryException.h"

#include <amqp.h>
#include <stdlib.h>

namespace AmqpClient {

AmqpLibraryException AmqpLibraryException::CreateException(int error_code) {
  std::string message(amqp_error_string2(error_code));

  return AmqpLibraryException(message, error_code);
}

AmqpLibraryException AmqpLibraryException::CreateException(
    int error_code, const std::string &context) {
  std::string message(context);
  message.append(": ");
  message.append(amqp_error_string2(error_code));

  return AmqpLibraryException(message, error_code);
}

AmqpLibraryException::AmqpLibraryException(const std::string &message,
                                           int error_code) throw()
    : std::runtime_error(message), m_errorCode(error_code) {}

}  
