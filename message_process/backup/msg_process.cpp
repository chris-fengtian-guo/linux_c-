#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

void monitorBehaviorTreeProcess(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);  
    if (WIFEXITED(status)) {
        int exitCode = WEXITSTATUS(status);
        std::cout << "Behavior Tree process exited with code: " << exitCode << std::endl;
    } else if (WIFSIGNALED(status)) {
        int signalNumber = WTERMSIG(status);
        std::cout << "Behavior Tree process terminated by signal: " << strsignal(signalNumber) << std::endl;
    } else {
        std::cout << "Behavior Tree process exited abnormally" << std::endl;
    }
}

int main() {
    
    pid_t pid = fork();
    if (pid == 0) {
        
        
        execl("/usr/bin/df", "behavior_tree_process", nullptr);
        exit(0);
    } else if (pid > 0) {
        
        std::thread monitorThread(monitorBehaviorTreeProcess, pid);

        
        

        
        monitorThread.join();
    } else {
        std::cerr << "Failed to fork a new process" << std::endl;
    }

    return 0;
}
