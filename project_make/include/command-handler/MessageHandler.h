
#pragma once

#include "Command.h"
#include <boost/process.hpp>

namespace bp = boost::process;

class MessageHandler {
public:
    MessageHandler();
    ~MessageHandler();
    void handleCommand(const Command& command);
    bp::child runBinaryWithArgument2(const char* binaryPath, const char* argument);
};
