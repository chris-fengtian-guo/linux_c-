#add_library(msg_process_lib 
#    YOUR_SOURCE_FILES_HERE.cpp
#)

# If you have executables for msg_process, for example:
#add_executable(msg_process_executable
#    recv_net_convert.cpp
#)

#target_link_libraries(msg_process_executable
#    msg_process_lib
#)
add_executable(msg_process_controller recv_net_convert.cpp)
target_compile_definitions(msg_process_controller PRIVATE BUILD_CONTROLLER=1)

# Second target: msg_process_device
add_executable(msg_process_device recv_net_convert.cpp)
target_compile_definitions(msg_process_device PRIVATE BUILD_DEVICE=1)
