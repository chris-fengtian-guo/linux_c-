#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
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
#include <pthread.h>
#include <chrono>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <chrono>


int main() {
    struct sockaddr_in sin = {0};
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(44444);
    inet_aton("127.0.0.1", &sin.sin_addr);  // Localhost for testing

    for (int i = 0; i < 5; ++i) {
        const char* msg = ("Hello " + std::to_string(i)).c_str();
        sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&sin, sizeof(sin));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(sock);
    return 0;
}
