

#include <iostream>

#include "connected_test.h"

using namespace AmqpClient;

TEST_F(connected_test, basic_ack_envelope) {
  const BasicMessage::ptr_t message = BasicMessage::Create("Message Body");
  std::string queue = channel->DeclareQueue("");
  channel->BasicPublish("", queue, message);

  std::string consumer = channel->BasicConsume(queue, "", true, false);

  Envelope::ptr_t env = channel->BasicConsumeMessage(consumer);

  channel->BasicAck(env);
}

TEST_F(connected_test, basic_ack_deliveryinfo) {
  const BasicMessage::ptr_t message = BasicMessage::Create("Message Body");
  std::string queue = channel->DeclareQueue("");
  channel->BasicPublish("", queue, message);

  std::string consumer = channel->BasicConsume(queue, "", true, false);

  Envelope::DeliveryInfo info;
  {
    Envelope::ptr_t env = channel->BasicConsumeMessage(consumer);
    info = env->GetDeliveryInfo();
  }

  channel->BasicAck(info);
}
