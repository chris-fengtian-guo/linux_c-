#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
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

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("239.255.255.250");
    addr.sin_port = htons(PORT);

    SharedData data;
    data.metadata["target_type"] = "some_type";
    data.metadata["position"] = "some_position";
    data.metadata["direction"] = "some_direction";

    std::stringstream ss;
    for(auto& kv: data.metadata)
    {
        ss << kv.first << ":" << kv.second << ",";
    }

    std::string serializedMetadata = ss.str();

    if (sendto(sock, serializedMetadata.c_str(), serializedMetadata.size(), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        perror("sendto");
        return 1;
    }

    close(sock);
    return 0;
}
