#ifndef CONNECTED_TEST_H
#define CONNECTED_TEST_H


#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <gtest/gtest.h>

using namespace AmqpClient;

class connected_test : public ::testing::Test {
 public:
  virtual void SetUp() { channel = Channel::Open(GetTestOpenOpts()); }

  Channel::ptr_t channel;

  static Channel::OpenOpts GetTestOpenOpts() {
    Channel::OpenOpts ret;
    ret.host = GetBrokerHost();
    ret.auth = Channel::OpenOpts::BasicAuth("guest", "guest");
    return ret;
  }

  static std::string GetBrokerHost() {
    const char *host = getenv("AMQP_BROKER");
    if (NULL != host) {
      return std::string(host);
    }
    return std::string("");
  }
};

#endif  
