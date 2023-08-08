#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#define MAX_BUFFER_SIZE 1024
#define MULTICAST_GROUP "239.0.0.1"
#define MULTICAST_PORT 8888

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr_send = {};
    addr_send.sin_family = AF_INET;
    addr_send.sin_port = htons(MULTICAST_PORT);
    addr_send.sin_addr.s_addr = inet_addr(MULTICAST_GROUP); 

    char buffer[MAX_BUFFER_SIZE] = "Hello, this is a multicast message!";
    if(sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&addr_send, sizeof(addr_send)) < 0) {
        perror("sendto");
        close(sock);
        return -1;
    }

    close(sock);
    return 0;
}
