#include "my_vector.h"  
#include <iostream>

int main() {
    my_vector<int> vec;  
    vec.push_back(1);  
    vec.push_back(2);
    vec.push_back(3);

    
    std::cout << "Size of vector: " << vec.size() << std::endl;
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << "Element " << i << ": " << vec.get(i) << std::endl;
    }

    return 0;
}
