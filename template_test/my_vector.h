#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <cstddef>

template <typename T>
class my_vector {
public:
    my_vector();  // constructor
    ~my_vector();  // destructor

    void push_back(const T& value);  // add an element
    T get(size_t index) const;  // get an element
    size_t size() const;  // get the size

private:
    T* array;  // dynamic array
    size_t array_size;  // size of the array
    size_t array_capacity;  // capacity of the array
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
        // Reallocate the array if it's too small
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
        // Out of bounds! Return a default value.
        return T();
    }
    return array[index];
}

template <typename T>
size_t my_vector<T>::size() const {
    return array_size;
}
#endif // MY_VECTOR_H

