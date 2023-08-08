#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

void workerFunction(int workerId) {
    std::cout << "Worker " << workerId << " is working.\n";
}

int main() {
    boost::asio::io_service ioService;
    boost::thread_group threadpool;

    
    
    boost::asio::io_service::work work(ioService);

    
    for(int i = 0; i < 2; i++) {
        threadpool.create_thread(
            boost::bind(&boost::asio::io_service::run, &ioService)
        );
    }

    
    for(int i = 0; i < 5; i++) {
        ioService.post(boost::bind(workerFunction, i));
    }

    
    
    

    
    threadpool.join_all();

    return 0;
}
