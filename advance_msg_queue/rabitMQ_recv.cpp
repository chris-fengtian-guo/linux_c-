#include <SimpleAmqpClient/SimpleAmqpClient.h>

int main()
{
    try
    {
        
        AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");

        
        std::string queueName = "test_queue";
        channel->DeclareQueue(queueName, false, true, false, false);

        
        std::string consumerTag = channel->BasicConsume(queueName);

        while (true)
        {
            
            AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumerTag);
            std::string message = envelope->Message()->Body();

            
            std::cout << "Received message: " << message << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
