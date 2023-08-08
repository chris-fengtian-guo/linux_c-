#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>

struct msg {
    unsigned int msg_type;
    std::string data;
};

msg receive_msg(int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    unsigned int msg_type_net;
    char buffer[1024];
    int n = recvfrom(sockfd, &msg_type_net, sizeof(msg_type_net), 0, NULL, NULL);

    
    unsigned int msg_type = ntohl(msg_type_net);

    n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, NULL, NULL);
    buffer[n] = '\0';

    msg received_msg;
    received_msg.msg_type = msg_type;
    received_msg.data = buffer;

    close(sockfd);

    return received_msg;
}

int main() {
#ifdef BUILD_CONTROLLER
	
	std::cout << "define BUILD_CONTROLLER \n";
#endif

#ifdef BUILD_DEVICE

	std::cout << "defiine BUILD_DEVICE \n";
#endif
    msg m = receive_msg(9000);
    std::cout << "Received msg_type: " << m.msg_type << ", data: " << m.data << std::endl;

    return 0;
}
