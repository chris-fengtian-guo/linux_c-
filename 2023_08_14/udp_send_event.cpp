#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <uv.h>

struct Message {
    std::string data;
    struct sockaddr_in dest_addr;
};

std::queue<Message> send_queue;
std::mutex mtx;
std::condition_variable cv;
bool running = true;

void sender_thread(uv_loop_t* loop, uv_udp_t* send_socket) {
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !send_queue.empty() || !running; });

        while (!send_queue.empty()) {
            Message msg = send_queue.front();
            send_queue.pop();

            uv_buf_t uv_msg = uv_buf_init(&msg.data[0], msg.data.size());
            uv_udp_send_t* send_req = new uv_udp_send_t;

            uv_udp_send(send_req, send_socket, &uv_msg, 1, (const struct sockaddr*)&msg.dest_addr,
                        [](uv_udp_send_t* req, int status) {
                            if (status) {
                                std::cerr << "Send error: " << uv_strerror(status) << std::endl;
                            }
                            delete req;
                        });
        }
    }
}

int main() {
    uv_loop_t* loop = uv_default_loop();

    uv_udp_t send_socket;
    uv_udp_init(loop, &send_socket);

    std::thread t(sender_thread, loop, &send_socket);

    // For testing purposes, we'll add a message to the send_queue:
    Message test_msg;
    test_msg.data = "Hello, World!";
    uv_ip4_addr("127.0.0.1", 8080, &test_msg.dest_addr);

    {
        std::lock_guard<std::mutex> lock(mtx);
        send_queue.push(test_msg);
        cv.notify_one();
    }

    uv_run(loop, UV_RUN_DEFAULT);

    running = false;
    t.join();

    uv_close((uv_handle_t*)&send_socket, nullptr);

    uv_loop_close(loop);
    free(loop);
    return 0;
}
