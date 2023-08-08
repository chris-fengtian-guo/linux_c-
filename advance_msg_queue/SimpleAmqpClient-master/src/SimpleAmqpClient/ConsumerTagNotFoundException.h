#ifndef SIMPLEAMQPCLIENT_CONSUMERTAGNOTFOUND_H
#define SIMPLEAMQPCLIENT_CONSUMERTAGNOTFOUND_H


#include <stdexcept>

#include "Util.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251 4275)
#endif




namespace AmqpClient {


class SIMPLEAMQPCLIENT_EXPORT ConsumerTagNotFoundException
    : public std::runtime_error {
 public:
  
  ConsumerTagNotFoundException() throw()
      : std::runtime_error("The specified consumer tag is unknown") {}
  
  virtual ~ConsumerTagNotFoundException() throw() {}
};

}  
#endif  
