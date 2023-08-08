

#include "connected_test.h"

using namespace AmqpClient;

TEST_F(connected_test, publish_success) {
  BasicMessage::ptr_t message = BasicMessage::Create("message body");

  channel->BasicPublish("", "test_publish_rk", message);
}

TEST(test_publish, publish_large_message) {
  
  Channel::OpenOpts opts = connected_test::GetTestOpenOpts();
  opts.frame_max = 4096;
  Channel::ptr_t channel = Channel::Open(opts);
  
  BasicMessage::ptr_t message = BasicMessage::Create(std::string(4099, 'a'));

  channel->BasicPublish("", "test_publish_rk", message);
}

TEST_F(connected_test, publish_badexchange) {
  BasicMessage::ptr_t message = BasicMessage::Create("message body");

  EXPECT_THROW(channel->BasicPublish("test_publish_notexist", "test_publish_rk",
                                     message),
               ChannelException);
}

TEST_F(connected_test, publish_recover_from_error) {
  BasicMessage::ptr_t message = BasicMessage::Create("message body");

  EXPECT_THROW(channel->BasicPublish("test_publish_notexist", "test_publish_rk",
                                     message),
               ChannelException);
  channel->BasicPublish("", "test_publish_rk", message);
}

TEST_F(connected_test, publish_mandatory_fail) {
  BasicMessage::ptr_t message = BasicMessage::Create("message body");

  EXPECT_THROW(
      channel->BasicPublish("", "test_publish_notexist", message, true),
      MessageReturnedException);
}

TEST_F(connected_test, publish_mandatory_success) {
  BasicMessage::ptr_t message = BasicMessage::Create("message body");
  std::string queue = channel->DeclareQueue("");

  channel->BasicPublish("", queue, message, true);
}

TEST_F(connected_test, publish_immediate_success) {
  BasicMessage::ptr_t message = BasicMessage::Create("message body");
  std::string queue = channel->DeclareQueue("");
  std::string consumer = channel->BasicConsume(queue, "");

  channel->BasicPublish("", queue, message, true);
}
