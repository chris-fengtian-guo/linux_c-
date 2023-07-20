#include <iostream>
#include <vector>
#include <string>
#include <map>

class SharedData {
public:
    int version;
    std::map<std::string, std::string> metadata; // Metadata for the data
    std::vector<uint8_t> binaryData; // Binary data

    // Default constructor
    SharedData() : version(0) {}

    // Function to update the data
    void updateData(const std::map<std::string, std::string>& newMetadata, const std::vector<uint8_t>& newBinaryData) {
        version++;
        metadata = newMetadata;
        binaryData = newBinaryData;
    }

    // Function to print the data
    void printData() {
        std::cout << "Version: " << version << std::endl;
        std::cout << "Metadata: " << std::endl;
        for(const auto& kv : metadata) {
            std::cout << kv.first << ": " << kv.second << std::endl;
        }
        std::cout << "Binary Data: ";
        for(auto data : binaryData) {
            std::cout << (int)data << " ";
        }
        std::cout << std::endl;
    }
};
