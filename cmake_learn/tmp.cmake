# First target: msg_process_controller
add_executable(msg_process_controller msg_process.cpp)
target_compile_definitions(msg_process_controller PRIVATE BUILD_CONTROLLER=1)

# Second target: msg_process_device
add_executable(msg_process_device msg_process.cpp)
target_compile_definitions(msg_process_device PRIVATE BUILD_DEVICE=1)
