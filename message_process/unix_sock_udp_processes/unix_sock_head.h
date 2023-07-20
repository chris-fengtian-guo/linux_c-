#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <thread>
#include <iostream>

#define UNIX_SRV_PATH "/tmp/unix_sock_server"
#include "common_head.h"

class UnixSockClient {
public:
    UnixSockClient() {
        sockfd_ = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sockfd_ == -1) {
            perror("socket");
        }
        server_addr_.sun_family = AF_UNIX;
        strcpy(server_addr_.sun_path, UNIX_SRV_PATH);
    }

    ~UnixSockClient() {
        close(sockfd_);
    }

    bool connect_to_server() {
        return (connect(sockfd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_)) != -1);
    }

    bool send_cmd(Command cmd) {
        int send_len = send(sockfd_, &cmd, sizeof(cmd), 0);
        if (send_len == -1) {
            perror("send");
            return false;
        }
        return true;
    }

private:
    int sockfd_;
    struct sockaddr_un server_addr_;
};

void send_cmd_in_thread(UnixSockClient& client) {
    Command cmd;
    cmd.action = 1;
    cmd.behaviorTreeID = 2;

    if (!client.send_cmd(cmd)) {
        std::cerr << "Failed to send command\n";
    } else {
        std::cout << "Command sent successfully\n";
    }
}

int main() {
    UnixSockClient client;

    if (!client.connect_to_server()) {
        std::cerr << "Failed to connect to server\n";
        return 1;
    }

    std::thread t(send_cmd_in_thread, std::ref(client));
    t.join();

    return 0;
}
