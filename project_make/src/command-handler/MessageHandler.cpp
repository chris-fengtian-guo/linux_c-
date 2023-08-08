
#include "MessageHandler.h"
#include <iostream>

MessageHandler::MessageHandler() {}

MessageHandler::~MessageHandler() {}

void MessageHandler::handleCommand(const Command& command) {
    
}

bp::child MessageHandler::runBinaryWithArgument2(const char* binaryPath, const char* argument) {
    
    bp::child c(bp::exe = binaryPath, bp::args = {argument});

    std::cout << "run process pid=" << c.id() << "\n, running=" << c.running() << "\n";
    c.wait();
    
    if (!c.running()) {
        std::cout << "The process has exited with status: " << c.exit_code() << std::endl;
    }

    return c;
}
void MessageHandler::handleCommand(const Command& command) {
    
    auto child = this->runBinaryWithArgument2(binaryPath.c_str(), argument.c_str());
}
