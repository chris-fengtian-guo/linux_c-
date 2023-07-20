#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>

#include "SharedData.h"

#define PORT 8888

int main()
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }

    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt");
        close(sock);
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(sock);
        return 1;
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt");
        close(sock);
        return 1;
    }

    char buf[1024];
    int recvlen = recv(sock, buf, 1024, 0);
    buf[recvlen] = 0;

    std::string serializedMetadata(buf);
    std::map<std::string, std::string> receivedMetadata;
    std::stringstream ss(serializedMetadata);
    std::string token;
    while(std::getline(ss, token, ','))
    {
        size_t pos = token.find(":");
        if(pos != std::string::npos)
        {
            receivedMetadata[token.substr(0, pos)] = token.substr(pos + 1);
        }
    }

    SharedData data;
    data.updateData(receivedMetadata, {});

    data.printData();

    close(sock);
    return 0;
}
