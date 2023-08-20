#include <iostream>
#include <string>
#include <uv.h>
#include <queue>
#include <mutex>
#include <unistd.h>
#include <cstring>

enum MessageType {
    MSG_REQUIRE,
    MSG_FILE
};

struct MsgHeader {
    MessageType type;
    uint32_t data_len;
    std::string msg_data;
};

std::queue<MsgHeader> data_queue;
std::mutex mtx;

void on_alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_read(uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags) {
    if (nread < 0) {
        std::cerr << "Read error: " << uv_strerror(nread) << std::endl;
        free(buf->base);
        uv_udp_recv_stop(handle);
        return;
    }

    if (nread > 0) {
        // Assume the received data is in the format of MsgHeader
        MsgHeader msg;
        memcpy(&msg.type, buf->base, sizeof(MessageType));
        memcpy(&msg.data_len, buf->base + sizeof(MessageType), sizeof(uint32_t));
        msg.msg_data = std::string(buf->base + sizeof(MessageType) + sizeof(uint32_t), nread - sizeof(MessageType) - sizeof(uint32_t));
        
        std::lock_guard<std::mutex> lock(mtx);
        data_queue.push(msg);
    }

    free(buf->base);
}

int main() {
    uv_loop_t *loop = uv_default_loop();

    uv_udp_t recv_socket;
    uv_udp_init(loop, &recv_socket);

    struct sockaddr_in recv_addr;
    uv_ip4_addr("0.0.0.0", 8080, &recv_addr);

    uv_udp_bind(&recv_socket, (const struct sockaddr*)&recv_addr, 0);
    uv_udp_recv_start(&recv_socket, on_alloc_buffer, on_read);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
    return 0;
}
