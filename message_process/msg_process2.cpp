#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define MAX_CLIENTS 10


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

void* handleClient(void* clientSocket) {
    int client = *(int*)clientSocket;
    int bytesRead;
    
    

    Command cmd;
    bytesRead = recv(client, &cmd, sizeof(cmd), 0);
    if (bytesRead == -1) {
        perror("recv");
    } else {
        
	std::cout << "receive len=" << bytesRead << "\n";
        std::cout << "Received command: " << cmd.action << " " << cmd.behaviorTreeID << std::endl;
    	
        const char* response = "Hello, client!";
        
    }
    close(client);
    free(clientSocket);

    if (bytesRead > 0) {
	const char* argument1 = "1";        
	
	
    	std::string binary1Path = executeBehaviorTree(cmd); 

    	std::cout << "Executed behavior tree path: " << binary1Path << std::endl;
    	const char* binary1PathChar = binary1Path.c_str();
    	int result1 = runBinaryWithArgument(binary1PathChar, binary1PathChar, argument1);

    } 
    return NULL;
}

int run_unix_server() {
    
    unlink(UNIX_SRV_PATH);
    int serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Failed to create server socket");
        return 1;
    }

    
    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, UNIX_SRV_PATH, sizeof(serverAddr.sun_path) - 1);

    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Failed to bind server socket");
        close(serverSocket);
        return 1;
    }

    
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Failed to listen on server socket");
        close(serverSocket);
        return 1;
    }

    while (1) {
        
        int* clientSocket = (int*)malloc(sizeof(int));
        if (clientSocket == NULL) {
            perror("Failed to allocate memory");
            continue;
        }
        *clientSocket = accept(serverSocket, NULL, NULL);
        if (*clientSocket == -1) {
            perror("Failed to accept client connection");
            free(clientSocket);
            continue;
        }

        
        pthread_t tid;
        if (pthread_create(&tid, NULL, handleClient, clientSocket) != 0) {
            perror("Failed to create thread");
            close(*clientSocket);
            free(clientSocket);
            continue;
        }
        pthread_detach(tid);
    }

    close(serverSocket);
    unlink(UNIX_SRV_PATH);

    return 0;
}
int main() {
    run_unix_server();
    
    return 0;
}
