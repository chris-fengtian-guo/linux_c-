#include <glog/logging.h>

int main(int argc, char* argv[]) {
    
    google::InitGoogleLogging(argv[0]);

    
    google::SetLogDestination(google::GLOG_INFO, "log_file.log");

    
    LOG(INFO) << "This is an INFO log";
    LOG(WARNING) << "This is a WARNING log";
    LOG(ERROR) << "This is an ERROR log";

    return 0;
}
