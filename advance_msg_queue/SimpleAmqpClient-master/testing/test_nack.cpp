

#include <iostream>

#include "connected_test.h"

using namespace AmqpClient;

TEST_F(connected_test, basic_nack_envelope) {
  const BasicMessage::ptr_t message = BasicMessage::Create("Message Body");
  std::string queue = channel->DeclareQueue("");
  channel->BasicPublish("", queue, message);

  std::string consumer = channel->BasicConsume(queue, "", true, false);

  Envelope::ptr_t env = channel->BasicConsumeMessage(consumer);

  channel->BasicReject(env, false);
}

TEST_F(connected_test, basic_nack_deliveryinfo) {
  const BasicMessage::ptr_t message = BasicMessage::Create("Message Body");
  std::string queue = channel->DeclareQueue("");
  channel->BasicPublish("", queue, message);

  std::string consumer = channel->BasicConsume(queue, "", true, false);

  Envelope::DeliveryInfo info;
  {
    Envelope::ptr_t env = channel->BasicConsumeMessage(consumer);
    info = env->GetDeliveryInfo();
  }

  channel->BasicReject(info, false);
}

TEST_F(connected_test, basic_nack_envelope_with_requeue) {
  const BasicMessage::ptr_t message = BasicMessage::Create("Message Body");
  std::string queue = channel->DeclareQueue("");
  channel->BasicPublish("", queue, message);

  std::string consumer = channel->BasicConsume(queue, "", true, false);

  Envelope::ptr_t env = channel->BasicConsumeMessage(consumer);

  channel->BasicReject(env, true);

  Envelope::ptr_t env2 = channel->BasicConsumeMessage(consumer);

  channel->BasicReject(env2, false);
}
