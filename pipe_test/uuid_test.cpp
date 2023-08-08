#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 
#include <boost/uuid/uuid_io.hpp>         
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>


int main() {
    boost::uuids::random_generator generator;
    boost::uuids::uuid uuid = generator();
    std::cout << uuid << std::endl;

    return 0;
}
