#ifndef SIMPLEAMQPCLIENT_BADURIEXCEPTION_H
#define SIMPLEAMQPCLIENT_BADURIEXCEPTION_H


#include <stdexcept>

#include "SimpleAmqpClient/Util.h"




namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT BadUriException : public std::runtime_error {
 public:
  explicit BadUriException() : std::runtime_error("URI is malformed") {}
};
}  
#endif  
