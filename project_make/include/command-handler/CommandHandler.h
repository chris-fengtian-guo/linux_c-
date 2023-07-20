#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "common_head.h"

class CommandHandler {
public:
    std::string executeBehaviorTree(const Command& command);
};

#endif
