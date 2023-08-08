#include <nlohmann/json.hpp>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>


int main() {
    
    nlohmann::json j;
    j["name"] = "John Doe";
    j["age"] = 30;
    j["is_student"] = false;
    j["courses"] = {"math", "english", "computer science"};

    
    std::string json_string = j.dump();
    std::cout << json_string << std::endl;

    return 0;
}
