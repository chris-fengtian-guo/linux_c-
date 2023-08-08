#include <SimpleAmqpClient/SimpleAmqpClient.h>

int main()
{
    try
    {
        
        AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");

        
        std::string queueName = "test_queue";
        channel->DeclareQueue(queueName, false, true, false, false);

        
        std::string message = "Hello, world!";
        channel->BasicPublish("", queueName, AmqpClient::BasicMessage::Create(message));
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
