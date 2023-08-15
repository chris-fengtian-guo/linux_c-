#ifndef MSG_PARSE_H
#define MSG_PARSE_H

#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

enum MessageType {
    MSG_REQUIRE = 0,
    MSG_FILE
};

struct MsgHeader {
    uint8_t msg_type;
    uint32_t data_len;
};

struct MsgRequire {
    nlohmann::json data;
};

struct MsgFile {
    std::string file_path;
    uint32_t file_size;
};

#endif
