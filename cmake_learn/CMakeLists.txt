cmake_minimum_required(VERSION 3.10)

project(distributed_manage)

# Set global output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Global include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add subprojects
add_subdirectory(source/msg_process)
add_subdirectory(source/pace_steper)
add_subdirectory(source/data_manage)
