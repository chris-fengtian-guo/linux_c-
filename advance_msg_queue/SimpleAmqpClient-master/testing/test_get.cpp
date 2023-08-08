

#include "connected_test.h"

using namespace AmqpClient;

TEST_F(connected_test, get_ok) {
  BasicMessage::ptr_t message = BasicMessage::Create("Message Body");
  std::string queue = channel->DeclareQueue("");
  channel->BasicPublish("", queue, message, true);

  Envelope::ptr_t new_message;
  EXPECT_TRUE(channel->BasicGet(new_message, queue));
  EXPECT_EQ(message->Body(), new_message->Message()->Body());
}

TEST_F(connected_test, get_empty) {
  BasicMessage::ptr_t message = BasicMessage::Create("Message Body");
  std::string queue = channel->DeclareQueue("");

  Envelope::ptr_t new_message;
  EXPECT_FALSE(channel->BasicGet(new_message, queue));
}

TEST(test_get, get_big) {
  
  Channel::OpenOpts opts = connected_test::GetTestOpenOpts();
  opts.frame_max = 4096;
  Channel::ptr_t channel = Channel::Open(opts);
  
  BasicMessage::ptr_t message = BasicMessage::Create(std::string(4099, 'a'));
  std::string queue = channel->DeclareQueue("");

  channel->BasicPublish("", queue, message);
  Envelope::ptr_t new_message;
  EXPECT_TRUE(channel->BasicGet(new_message, queue));
  EXPECT_EQ(message->Body(), new_message->Message()->Body());
}

TEST_F(connected_test, bad_queue) {
  Envelope::ptr_t new_message;
  EXPECT_THROW(channel->BasicGet(new_message, "test_get_nonexistantqueue"),
               ChannelException);
}

TEST_F(connected_test, ack_message) {
  BasicMessage::ptr_t message = BasicMessage::Create("Message Body");
  std::string queue = channel->DeclareQueue("");
  channel->BasicPublish("", queue, message, true);

  Envelope::ptr_t new_message;
  EXPECT_TRUE(channel->BasicGet(new_message, queue, false));
  channel->BasicAck(new_message);
  EXPECT_FALSE(channel->BasicGet(new_message, queue, false));
}
