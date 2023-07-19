#ifndef COMMAND_H
#define COMMAND_H

#include <string>

#define UNIX_SRV_PATH "/tmp/my_socket"


#include <boost/serialization/access.hpp>

struct Command {
    unsigned int action; // 动作
    unsigned int behaviorTreeID; // 行为树ID

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & action;
        ar & behaviorTreeID;
    }
};
#endif // COMMAND_H
