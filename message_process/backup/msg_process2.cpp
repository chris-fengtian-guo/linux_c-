#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector> 

#include "common_head.h"


int runBinaryWithArgument(const char* binaryPath, const char* name, const char* argument) {
    pid_t pid = fork();
    if (pid == 0) {
        
        execlp(binaryPath, name, argument, nullptr);
        exit(0);
    } else if (pid > 0) {
        
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exitStatus = WEXITSTATUS(status);
            std::cout << "Binary " << binaryPath << " exited with status: " << exitStatus << std::endl;
            return exitStatus;
        } else if (WIFSIGNALED(status)) {
            int signalCode = WTERMSIG(status);
            std::cout << "Binary " << binaryPath << " terminated by signal: " << signalCode << std::endl;
            return -signalCode;
        } else {
            std::cout << "Binary " << binaryPath << " terminated abnormally" << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Failed to fork a new process" << std::endl;
        return -1;
    }
}


const std::vector<std::string> behaviorTreePaths = {
    "/root/autodl-tmp/linux_c++/message_process/behavior_tree_style1",
    "/root/autodl-tmp/linux_c++/message_process/behavior_tree_style2"
};


std::string executeBehaviorTree(const Command& command) {
    if (command.behaviorTreeID < behaviorTreePaths.size()) {
        const std::string& behaviorTreePath = behaviorTreePaths[command.behaviorTreeID];
        
        std::cout << "Executing behavior tree at path: " << behaviorTreePath << std::endl;
        return behaviorTreePath;
    } else {
        std::cerr << "Invalid behavior tree ID: " << command.behaviorTreeID << std::endl;
        return ""; 
    }
}

int main() {
    const char* argument1 = "1";        

    const char* binary2Path = "/root/autodl-tmp/linux_c++/message_process/process_exit_example"; 
    const char* argument2 = "2";        
    const char* name = "message_process";
    
    std::string action = "run";
    unsigned int behaviorTreeID = 0; 

    
    Command cmd;
    cmd.action = action;
    cmd.behaviorTreeID = behaviorTreeID;

    
    std::string binary1Path = executeBehaviorTree(cmd); 

    std::cout << "Executed behavior tree path: " << binary1Path << std::endl;
    const char* binary1PathChar = binary1Path.c_str();

    int result1 = runBinaryWithArgument(binary1PathChar, name, argument1);
    

    return 0;
}
