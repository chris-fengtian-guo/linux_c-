

#include <amqp.h>

#include <algorithm>
#include <boost/array.hpp>
#include <iostream>

#include "connected_test.h"

using namespace AmqpClient;

TEST(basic_message, empty_message) {
  BasicMessage::ptr_t empty_message = BasicMessage::Create();

  EXPECT_EQ(std::string(), empty_message->Body());

  
}

TEST(basic_message, empty_message_add_body) {
  BasicMessage::ptr_t empty_message = BasicMessage::Create();

  EXPECT_EQ(std::string(), empty_message->Body());

  const std::string body("Message Body");
  empty_message->Body(body);

  EXPECT_EQ(body, empty_message->Body());

  
}

TEST(basic_message, empty_message_add_body2) {
  BasicMessage::ptr_t empty_message = BasicMessage::Create();

  EXPECT_EQ(std::string(), empty_message->Body());

  const std::string body("Message Body");
  empty_message->Body(body);

  EXPECT_EQ(body, empty_message->Body());

  const std::string body2("Second body");
  empty_message->Body(body2);
  EXPECT_EQ(body2, empty_message->Body());

  
}

TEST(basic_message, initial_message_replace) {
  const std::string first_body("First message Body");
  BasicMessage::ptr_t message = BasicMessage::Create(first_body);

  EXPECT_EQ(first_body, message->Body());

  const std::string second_body("Second message Body");
  message->Body(second_body);

  EXPECT_EQ(second_body, message->Body());

  
}

TEST(basic_message, initial_message_replace2) {
  const std::string first_body("First message body");
  BasicMessage::ptr_t message = BasicMessage::Create(first_body);
  EXPECT_EQ(first_body, message->Body());

  const std::string second_body("second message body");
  message->Body(second_body);
  EXPECT_EQ(second_body, message->Body());

  const std::string third_body("3rd Body");
  message->Body(third_body);
  EXPECT_EQ(third_body, message->Body());
}

TEST(basic_message, embedded_nulls) {
  const boost::array<char, 7> message_data = {
      {'a', 'b', 'c', 0, '1', '2', '3'}};
  const std::string body(message_data.data(), message_data.size());
  BasicMessage::ptr_t message = BasicMessage::Create(body);
  EXPECT_EQ(body, message->Body());

  const boost::array<char, 7> message_data2 = {
      {'1', '2', '3', 0, 'a', 'b', 'c'}};
  const std::string body2(message_data2.data(), message_data2.size());
  message->Body(body2);
  EXPECT_EQ(body2, message->Body());
}

TEST_F(connected_test, replaced_received_body) {
  const std::string queue = channel->DeclareQueue("");
  const std::string consumer = channel->BasicConsume(queue);

  const std::string body("First Message Body");
  BasicMessage::ptr_t out_message = BasicMessage::Create(body);
  channel->BasicPublish("", queue, out_message);

  Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer);
  BasicMessage::ptr_t in_message = envelope->Message();
  EXPECT_EQ(out_message->Body(), in_message->Body());

  const std::string body2("Second message body");
  in_message->Body(body2);
  EXPECT_EQ(body2, in_message->Body());
}
