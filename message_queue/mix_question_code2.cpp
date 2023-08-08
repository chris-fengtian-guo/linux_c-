#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " device_type" << std::endl;
        return 1;
    }

    std::string device_type = argv[1];
    if (device_type == "controller") {
        
    } else if (device_type == "sweep_robot") {
        
    } else {
        std::cerr << "Unknown device type: " << device_type << std::endl;
        return 1;
    }

    return 0;
}
