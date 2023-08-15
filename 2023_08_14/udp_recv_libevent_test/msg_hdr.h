#include <iostream>
#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

enum MessageType {
    MSG_REQUIRE,
    MSG_FILE,
    MSG_TASK,
    MSG_DEVICE_STATUS
};

struct MsgHeader {
    MessageType type;
    uint32_t data_len;
};

struct MsgRequire {
    nlohmann::json data;
};

struct MsgFile {
    std::string file_path;
    uint64_t file_size;
};

struct MsgTask {
    std::string task_info;  // 可以根据实际需求修改此字段
};

struct MsgDeviceStatus {
    std::string device_info;  // 可以根据实际需求修改此字段
};

// 以下是解析报文的函数
void parseMessage(const char* buffer, size_t len) {
    if (len < sizeof(MsgHeader)) {
        std::cerr << "Invalid message length!" << std::endl;
        return;
    }

    MsgHeader* header = (MsgHeader*)buffer;
    switch (header->type) {
        case MSG_REQUIRE: {
            MsgRequire msg;
            msg.data = nlohmann::json::parse(buffer + sizeof(MsgHeader));
            // 处理msg.data ...
            break;
        }
        case MSG_FILE: {
            MsgFile msg;
            // 这里只是一个示例，需要根据实际报文格式解析
            msg.file_path = std::string(buffer + sizeof(MsgHeader));
            // 假设文件大小紧跟在路径之后
            msg.file_size = *(uint64_t*)(buffer + sizeof(MsgHeader) + msg.file_path.size() + 1);
            // 处理msg ...
            break;
        }
        case MSG_TASK: {
            MsgTask msg;
            msg.task_info = std::string(buffer + sizeof(MsgHeader));
            // 处理msg ...
            break;
        }
        case MSG_DEVICE_STATUS: {
            MsgDeviceStatus msg;
            msg.device_info = std::string(buffer + sizeof(MsgHeader));
            // 处理msg ...
            break;
        }
        default: {
            std::cerr << "Unknown message type!" << std::endl;
        }
    }
}


