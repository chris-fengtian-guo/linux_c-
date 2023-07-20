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

    struct sockaddr_in addr_recv = {};
    addr_recv.sin_family = AF_INET;
    addr_recv.sin_port = htons(MULTICAST_PORT);
    addr_recv.sin_addr.s_addr = htonl(INADDR_ANY); // any address

    if(bind(sock, (struct sockaddr*)&addr_recv, sizeof(addr_recv)) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        close(sock);
        return -1;
    }

    char buffer[MAX_BUFFER_SIZE] = {0};
    if(recv(sock, buffer, MAX_BUFFER_SIZE - 1, 0) < 0) {
        perror("recv");
        close(sock);
        return -1;
    }

    printf("Received message: %s\n", buffer);

    close(sock);
    return 0;
}
