#include <glog/logging.h>
#include <signal.h>
#include <unistd.h>

// 信号处理函数
void SignalHandler(int signal) {
    if (signal == SIGUSR1) {
        // 提高日志级别
        FLAGS_minloglevel++;
        LOG(INFO) << "Increased log level to: " << FLAGS_minloglevel;
    } else if (signal == SIGUSR2) {
        // 降低日志级别
        FLAGS_minloglevel = std::max(0, FLAGS_minloglevel - 1);
        LOG(INFO) << "Decreased log level to: " << FLAGS_minloglevel;
    }
}

int main(int argc, char* argv[]) {
    // 初始化 Google's logging library.
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO, "./info_");
    FLAGS_logtostderr = true;  // 日志输出到标准错误

    // 注册信号处理函数
    signal(SIGUSR1, SignalHandler);
    signal(SIGUSR2, SignalHandler);

    LOG(INFO) << "Program started with log level: " << FLAGS_minloglevel;

    // 循环保持程序继续运行
    while (true) {
        LOG(INFO) << "This is an INFO log";
        LOG(WARNING) << "This is a WARNING log";
        LOG(ERROR) << "This is an ERROR log";
        sleep(5);  // 每 5 秒打印一次日志
    }

    return 0;
}
