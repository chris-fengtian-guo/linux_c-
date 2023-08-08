#ifndef COMMAND_H
#define COMMAND_H

#include <string>


struct Command {
    unsigned int action; 
    unsigned int behaviorTreeID; 
};
#define UNIX_SRV_PATH "/tmp/my_socket"

#endif 
