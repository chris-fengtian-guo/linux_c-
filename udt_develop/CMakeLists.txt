cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
project(udt_file_transfer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# UDT库的相对路径
set(UDT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(UDT_LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

include_directories(${UDT_INCLUDE_DIR})
link_directories(${UDT_LIB_DIR})

add_executable(send_file send_file.cpp)
add_executable(receive_file receive_file.cpp)

target_link_libraries(send_file udt pthread m)
target_link_libraries(receive_file udt pthread m)
