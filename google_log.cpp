#include <glog/logging.h>

int main(int argc, char* argv[]) {
    // 初始化 Google's logging library.
    google::InitGoogleLogging(argv[0]);

    // 设置日志输出到文件
    google::SetLogDestination(google::GLOG_INFO, "log_file.log");

    // 记录一些基本日志
    LOG(INFO) << "This is an INFO log";
    LOG(WARNING) << "This is a WARNING log";
    LOG(ERROR) << "This is an ERROR log";

    return 0;
}
