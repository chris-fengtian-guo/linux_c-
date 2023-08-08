#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


struct Command {
    std::string action; 
    int behaviorTreeID; 
};

int main() {
    
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/unix_socket_server"); 

    
    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    
    if (listen(server_fd, 1) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Waiting for connection..." << std::endl;

    sockaddr_un client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    
    int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_addr_len);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    
    Command cmd;
    if (recv(client_fd, &cmd, sizeof(cmd), 0) == -1) {
        perror("recv");
    } else {
        
        std::cout << "Received command: " << cmd.action << " " << cmd.behaviorTreeID << std::endl;
    }

    
    close(client_fd);
    close(server_fd);

    return 0;
}
