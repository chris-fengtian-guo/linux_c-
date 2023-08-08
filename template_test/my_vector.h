#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <cstddef>

template <typename T>
class my_vector {
public:
    my_vector();  
    ~my_vector();  

    void push_back(const T& value);  
    T get(size_t index) const;  
    size_t size() const;  

private:
    T* array;  
    size_t array_size;  
    size_t array_capacity;  
};

template <typename T>
my_vector<T>::my_vector() {
    array = new T[1];
    array_size = 0;
    array_capacity = 1;
}

template <typename T>
my_vector<T>::~my_vector() {
    delete[] array;
}

template <typename T>
void my_vector<T>::push_back(const T& value) {
    if (array_size == array_capacity) {
        
        T* new_array = new T[array_capacity * 2];
        for (size_t i = 0; i < array_size; i++) {
            new_array[i] = array[i];
        }
        delete[] array;
        array = new_array;
        array_capacity *= 2;
    }
    array[array_size++] = value;
}

template <typename T>
T my_vector<T>::get(size_t index) const {
    if (index >= array_size) {
        
        return T();
    }
    return array[index];
}

template <typename T>
size_t my_vector<T>::size() const {
    return array_size;
}
#endif 

