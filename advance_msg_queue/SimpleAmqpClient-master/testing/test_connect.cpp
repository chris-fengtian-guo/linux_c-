

#include <gtest/gtest.h>

#include "SimpleAmqpClient/SimpleAmqpClient.h"
#include "connected_test.h"

using namespace AmqpClient;

TEST(connecting_test, connect_default) {
  Channel::ptr_t channel = Channel::Create(connected_test::GetBrokerHost());
}

TEST(connecting_test, connect_badhost) {
  EXPECT_THROW(Channel::ptr_t channel = Channel::Create("HostDoesntExist"),
               std::runtime_error);
}

TEST(connecting_test, open_badhost) {
  Channel::OpenOpts opts = connected_test::GetTestOpenOpts();
  opts.host = "HostDoesNotExist";
  EXPECT_THROW(Channel::ptr_t channel = Channel::Open(opts),
               std::runtime_error);
}

TEST(connecting_test, connect_badauth) {
  EXPECT_THROW(Channel::ptr_t channel = Channel::Create(
                   connected_test::GetBrokerHost(), 5672, "baduser", "badpass"),
               AccessRefusedException);
}

TEST(connecting_test, open_badauth) {
  Channel::OpenOpts opts = connected_test::GetTestOpenOpts();
  opts.auth = Channel::OpenOpts::BasicAuth("baduser", "badpass");
  EXPECT_THROW(Channel::ptr_t channel = Channel::Open(opts),
               AccessRefusedException);
}

TEST(connecting_test, connect_badframesize) {
  
  EXPECT_THROW(
      Channel::ptr_t channel = Channel::Create(
          connected_test::GetBrokerHost(), 5672, "guest", "guest", "/", 400),
      AmqpResponseLibraryException);
}

TEST(connecting_test, open_badframesize) {
  
  Channel::OpenOpts opts = connected_test::GetTestOpenOpts();
  opts.frame_max = 400;
  EXPECT_THROW(Channel::ptr_t channel = Channel::Open(opts),
               AmqpResponseLibraryException);
}

TEST(connecting_test, connect_badvhost) {
  EXPECT_THROW(Channel::ptr_t channel =
                   Channel::Create(connected_test::GetBrokerHost(), 5672,
                                   "guest", "guest", "nonexitant_vhost"),
               NotAllowedException);
}

TEST(connecting_test, open_badvhost) {
  Channel::OpenOpts opts = connected_test::GetTestOpenOpts();
  opts.vhost = "bad_vhost";
  EXPECT_THROW(Channel::ptr_t channel = Channel::Open(opts),
               NotAllowedException);
}

TEST(connecting_test, connect_using_uri) {
  std::string host_uri = "amqp:
  Channel::ptr_t channel = Channel::CreateFromUri(host_uri);
}

TEST(connecting_test, openopts_from_uri) {
  Channel::OpenOpts expected;
  expected.host = "host";
  expected.vhost = "vhost";
  expected.port = 123;
  expected.auth = Channel::OpenOpts::BasicAuth("user", "pass");

  EXPECT_EQ(expected,
            Channel::OpenOpts::FromUri("amqp:
}

TEST(connecting_test, openopts_from_uri_defaults) {
  Channel::OpenOpts expected;
  expected.host = "host";
  expected.vhost = "/";
  expected.port = 5672;
  expected.auth = Channel::OpenOpts::BasicAuth("guest", "guest");
  EXPECT_EQ(expected, Channel::OpenOpts::FromUri("amqp:
}

TEST(connecting_test, openopts_from_amqps_uri) {
  Channel::OpenOpts expected;
  expected.host = "host";
  expected.vhost = "vhost";
  expected.port = 123;
  expected.auth = Channel::OpenOpts::BasicAuth("user", "pass");
  expected.tls_params = Channel::OpenOpts::TLSParams();
}

TEST(connecting_test, openopts_fromuri_bad) {
  EXPECT_THROW(Channel::OpenOpts::FromUri("not-a-valid-uri"), BadUriException);
}
