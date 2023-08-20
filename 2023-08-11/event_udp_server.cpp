#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <event2/event.h>
#include <event2/buffer.h>
#include <netinet/in.h>

void udp_recv_cb(const int sock, short int, void *arg) {
    char buf[512];
    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);

    int recv_len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&sender_addr, &addr_len);
    if (recv_len > 0) {
        buf[recv_len] = 0; // Null terminate the received string
        std::cout << "Received: " << buf << std::endl;
    }
}

int main() {
    struct sockaddr_in sin = {0};
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(0);
    sin.sin_port = htons(44444);

    bind(sock, (struct sockaddr*)&sin, sizeof(sin));

    struct event_base *base = event_base_new();
    struct event *listen_ev = event_new(base, sock, EV_READ | EV_PERSIST, udp_recv_cb, NULL);

    event_add(listen_ev, NULL);
    event_base_dispatch(base);

    // Cleanup
    event_free(listen_ev);
    event_base_free(base);
    close(sock);

    return 0;
}
