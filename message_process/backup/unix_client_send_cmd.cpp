#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>


struct Command {
    std::string action; 
    int behaviorTreeID; 
};

int main() {
    
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/unix_socket_server"); 

    
    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    
    std::string action = "run";
    int behaviorTreeID = 0;

    
    Command cmd;
    cmd.action = action;
    cmd.behaviorTreeID = behaviorTreeID;

    
    if (send(sockfd, &cmd, sizeof(cmd), 0) == -1) {
        perror("send");
    }

    
    close(sockfd);

    return 0;
}
