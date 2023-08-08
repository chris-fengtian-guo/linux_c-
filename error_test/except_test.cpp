#include <iostream>
#include <fstream>
#include <exception>

int main() {
    std::ifstream file;

    try {
        file.open("non_existent_file.txt");
        if (!file) {
            throw std::runtime_error("Error opening the file");
        }
        
    } 
    catch (std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
    }

    return 0;
}
