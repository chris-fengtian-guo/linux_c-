#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    // 打开并读取配置文件
    std::ifstream config_file("config.json");
    if (!config_file.is_open()) {
        std::cerr << "无法打开配置文件!" << std::endl;
        return -1;
    }

    json config;
    config_file >> config;

    int udp_port = config["udp"]["port"];
    int send_queue_size = config["udp"]["queue_sizes"]["send_queue"];
    int receive_queue_size = config["udp"]["queue_sizes"]["receive_queue"];
    int processing_queue_size = config["udp"]["queue_sizes"]["processing_queue"];
    int thread_count = config["threads"]["count"];

    std::cout << "UDP Port: " << udp_port << std::endl;
    std::cout << "Send Queue Size: " << send_queue_size << std::endl;
    std::cout << "Receive Queue Size: " << receive_queue_size << std::endl;
    std::cout << "Processing Queue Size: " << processing_queue_size << std::endl;
    std::cout << "Thread Count: " << thread_count << std::endl;

    return 0;
}
