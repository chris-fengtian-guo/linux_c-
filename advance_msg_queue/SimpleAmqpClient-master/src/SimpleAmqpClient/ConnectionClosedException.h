#ifndef SIMPLEAMQPCLIENT_CONNECTIONCLOSEDEXCEPTION_H
#define SIMPLEAMQPCLIENT_CONNECTIONCLOSEDEXCEPTION_H


#include <stdexcept>

#include "SimpleAmqpClient/Util.h"




namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT ConnectionClosedException
    : public std::runtime_error {
 public:
  
  explicit ConnectionClosedException()
      : std::runtime_error("Connection is closed") {}
};
}  
#endif  
