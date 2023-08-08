#include "include/command-handler/CommandHandler.h"
#include <vector>
#include <iostream>


const std::vector<std::string> behaviorTreePaths = {
    "/root/autodl-tmp/linux_c++/message_process/behavior_tree_style1",
    "/root/autodl-tmp/linux_c++/message_process/behavior_tree_style2"
};

std::string CommandHandler::executeBehaviorTree(const Command& command) {
    if (command.behaviorTreeID < behaviorTreePaths.size()) {
        const std::string& behaviorTreePath = behaviorTreePaths[command.behaviorTreeID];
        
        std::cout << "Executing behavior tree at path: " << behaviorTreePath << std::endl;
        return behaviorTreePath;
    } else {
        std::cerr << "Invalid behavior tree ID: " << command.behaviorTreeID << std::endl;
        return ""; 
    }
}
