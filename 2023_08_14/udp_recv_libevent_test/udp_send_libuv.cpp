#include <iostream>
#include <uv.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <uv.h>
#include <queue>
#include <mutex>
#include "msg_hdr.h"

std::queue<MsgHeader> data_queue;
std::mutex mtx;


void send_msg(uv_udp_t* client, const sockaddr* dest, const char* buffer, size_t len) {
    uv_buf_t buf = uv_buf_init((char*)buffer, len);
    uv_udp_send_t* send_req = new uv_udp_send_t;

    uv_udp_send(send_req, client, &buf, 1, dest, [](uv_udp_send_t* req, int status) {
        if (status) {
            std::cerr << "Send error: " << uv_strerror(status) << std::endl;
        }
        delete req;
    });
}

int main() {
    uv_loop_t* loop = uv_default_loop();

    uv_udp_t client;
    uv_udp_init(loop, &client);

    sockaddr_in dest;
    uv_ip4_addr("127.0.0.1", 12345, &dest); // 目标地址

    // 发送 MSG_REQUIRE
    {
        //MsgHeader header = {MSG_REQUIRE, sizeof(nlohmann::json)};
        MsgHeader header = {MSG_REQUIRE, 0};  // Use msg_serialized.size() instead of sizeof(nlohmann::json)
        MsgRequire require_msg = {{ {"key", "value"} }}; // 举例的JSON数据

        std::string msg_serialized = require_msg.data.dump();
        char buffer[sizeof(MsgHeader) + msg_serialized.size()];
	header.data_len = msg_serialized.size();
        memcpy(buffer, &header, sizeof(MsgHeader));
        memcpy(buffer + sizeof(MsgHeader), msg_serialized.c_str(), msg_serialized.size());

        send_msg(&client, (const sockaddr*)&dest, buffer, sizeof(buffer));
    }

    // 发送 MSG_FILE
    {
        MsgHeader header = {MSG_FILE, sizeof(std::string) + sizeof(uint64_t)};
        MsgFile file_msg = {"path/to/file.txt", 12345}; // 举例的文件路径和大小

        char buffer[sizeof(MsgHeader) + sizeof(std::string) + sizeof(uint64_t)];
        memcpy(buffer, &header, sizeof(MsgHeader));
        memcpy(buffer + sizeof(MsgHeader), file_msg.file_path.c_str(), file_msg.file_path.size());
        memcpy(buffer + sizeof(MsgHeader) + file_msg.file_path.size(), &file_msg.file_size, sizeof(uint64_t));

        send_msg(&client, (const sockaddr*)&dest, buffer, sizeof(buffer));
    }

    uv_run(loop, UV_RUN_DEFAULT);
    uv_close((uv_handle_t*)&client, nullptr);
    uv_loop_close(loop);

    return 0;
}
