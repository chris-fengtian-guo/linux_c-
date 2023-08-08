


#include "SimpleAmqpClient/AmqpResponseLibraryException.h"

#include <amqp.h>
#include <stdlib.h>

namespace AmqpClient {

AmqpResponseLibraryException AmqpResponseLibraryException::CreateException(
    const amqp_rpc_reply_t_ &reply, const std::string &context) {
  std::string message(context);
  message.append(": ");
  message.append(amqp_error_string2(reply.library_error));

  return AmqpResponseLibraryException(message);
}

AmqpResponseLibraryException::AmqpResponseLibraryException(
    const std::string &message) throw()
    : std::runtime_error(message) {}

}  
