#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>

#define MAX_BUFFER_SIZE 1024
#define PORT 8080

enum MessageType {
    FILE_TRANSFER,
    COMMAND_INFO,
    DEVICE_STATUS
};

struct Message {
    MessageType type;
    std::string uuid;
    std::string device_id;
    std::string content;

    
    static Message fromString(const std::string& str);
};

Message Message::fromString(const std::string& str) {
    Message msg;
    std::string token;
    std::istringstream tokenStream(str);
    std::vector<std::string> tokens;

    while (std::getline(tokenStream, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 4) {
        msg.type = static_cast<MessageType>(std::stoi(tokens[0])); 
        msg.uuid = tokens[1];
        msg.device_id = tokens[2];
        msg.content = tokens[3];
    }

    return msg;
}
typedef void (*CallbackFunc)(const Message&);

struct Registration {
    std::string uuid;
    CallbackFunc callback;
};

class CallbackManager {
public:
    void registerCallback(const std::string& uuid, CallbackFunc callback) {
        registrations_.push_back({uuid, callback});
    }

    void notify(const Message& msg) {
        for (const auto& reg : registrations_) {
	    std::cout << "callback manager receive msg reg.uuid=" << reg.uuid << " msg.uuid=" << msg.uuid << "\n";
            if (reg.uuid == msg.uuid) {
                reg.callback(msg);
            }
        }
    }

private:
    std::vector<Registration> registrations_;
};

std::queue<Message> messageQueue; 
std::mutex queueMutex;
std::condition_variable dataAvailable;
CallbackManager callbackManager; 

void udp_receiver() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAX_BUFFER_SIZE];

    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&cliaddr, &len);
        buffer[n] = '\0';

        std::unique_lock<std::mutex> lock(queueMutex);
        std::cout << "recvfrom Received Message: " << buffer << std::endl;

        Message msg = Message::fromString(std::string(buffer));
        messageQueue.push(msg);
        dataAvailable.notify_one();
    }

    close(sockfd);
}

void message_processor() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        dataAvailable.wait(lock, [] { return !messageQueue.empty(); });

        Message msg = messageQueue.front();
        messageQueue.pop();

        
        callbackManager.notify(msg);
    }
}
void paceStepperHandler(const Message& msg) {
    
    std::cout << "PaceStepper received message with content: " << msg.content << std::endl;
}

int main() {
    callbackManager.registerCallback("1234-5678", paceStepperHandler);

    std::thread receiverThread(udp_receiver);
    std::thread processorThread(message_processor);

    receiverThread.join();
    processorThread.join();

    return 0;
}
