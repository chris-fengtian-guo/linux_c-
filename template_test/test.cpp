#include "my_vector.h"  // include the header file with the template class
#include <iostream>

int main() {
    my_vector<int> vec;  // create a my_vector of ints
    vec.push_back(1);  // add some values
    vec.push_back(2);
    vec.push_back(3);

    // print the size and contents of the vector
    std::cout << "Size of vector: " << vec.size() << std::endl;
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << "Element " << i << ": " << vec.get(i) << std::endl;
    }

    return 0;
}
