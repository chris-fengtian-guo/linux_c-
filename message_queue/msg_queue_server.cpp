#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>

int main()
{
    using namespace boost::interprocess;

    try{
        
        message_queue::remove("message_queue");

        
        message_queue mq
        (create_only               
        ,"message_queue"           
        ,100                       
        ,sizeof(int)               
        );

        
        for(int i = 0; i < 10; ++i){
            mq.send(&i, sizeof(i), 0);
        }

        
        unsigned int priority;
        boost::interprocess::message_queue::size_type recvd_size;
        for(int i = 0; i < 10; ++i){
            int number;
            mq.receive(&number, sizeof(number), recvd_size, priority);
            std::cout << "Received number " << number << std::endl;
        }
    }
    catch(interprocess_exception &ex){
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
