# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /root/miniconda3/lib/python3.8/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /root/miniconda3/lib/python3.8/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/autodl-tmp/linux_c++/UDT-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/autodl-tmp/linux_c++/UDT-master/build

# Include any dependencies generated for this target.
include CMakeFiles/udt.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/udt.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/udt.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/udt.dir/flags.make

CMakeFiles/udt.dir/src/api.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/api.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/api.cpp
CMakeFiles/udt.dir/src/api.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/udt.dir/src/api.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/api.cpp.o -MF CMakeFiles/udt.dir/src/api.cpp.o.d -o CMakeFiles/udt.dir/src/api.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/api.cpp

CMakeFiles/udt.dir/src/api.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/api.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/api.cpp > CMakeFiles/udt.dir/src/api.cpp.i

CMakeFiles/udt.dir/src/api.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/api.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/api.cpp -o CMakeFiles/udt.dir/src/api.cpp.s

CMakeFiles/udt.dir/src/buffer.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/buffer.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/buffer.cpp
CMakeFiles/udt.dir/src/buffer.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/udt.dir/src/buffer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/buffer.cpp.o -MF CMakeFiles/udt.dir/src/buffer.cpp.o.d -o CMakeFiles/udt.dir/src/buffer.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/buffer.cpp

CMakeFiles/udt.dir/src/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/buffer.cpp > CMakeFiles/udt.dir/src/buffer.cpp.i

CMakeFiles/udt.dir/src/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/buffer.cpp -o CMakeFiles/udt.dir/src/buffer.cpp.s

CMakeFiles/udt.dir/src/cache.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/cache.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/cache.cpp
CMakeFiles/udt.dir/src/cache.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/udt.dir/src/cache.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/cache.cpp.o -MF CMakeFiles/udt.dir/src/cache.cpp.o.d -o CMakeFiles/udt.dir/src/cache.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/cache.cpp

CMakeFiles/udt.dir/src/cache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/cache.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/cache.cpp > CMakeFiles/udt.dir/src/cache.cpp.i

CMakeFiles/udt.dir/src/cache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/cache.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/cache.cpp -o CMakeFiles/udt.dir/src/cache.cpp.s

CMakeFiles/udt.dir/src/ccc.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/ccc.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/ccc.cpp
CMakeFiles/udt.dir/src/ccc.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/udt.dir/src/ccc.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/ccc.cpp.o -MF CMakeFiles/udt.dir/src/ccc.cpp.o.d -o CMakeFiles/udt.dir/src/ccc.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/ccc.cpp

CMakeFiles/udt.dir/src/ccc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/ccc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/ccc.cpp > CMakeFiles/udt.dir/src/ccc.cpp.i

CMakeFiles/udt.dir/src/ccc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/ccc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/ccc.cpp -o CMakeFiles/udt.dir/src/ccc.cpp.s

CMakeFiles/udt.dir/src/channel.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/channel.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/channel.cpp
CMakeFiles/udt.dir/src/channel.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/udt.dir/src/channel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/channel.cpp.o -MF CMakeFiles/udt.dir/src/channel.cpp.o.d -o CMakeFiles/udt.dir/src/channel.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/channel.cpp

CMakeFiles/udt.dir/src/channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/channel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/channel.cpp > CMakeFiles/udt.dir/src/channel.cpp.i

CMakeFiles/udt.dir/src/channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/channel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/channel.cpp -o CMakeFiles/udt.dir/src/channel.cpp.s

CMakeFiles/udt.dir/src/core.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/core.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/core.cpp
CMakeFiles/udt.dir/src/core.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/udt.dir/src/core.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/core.cpp.o -MF CMakeFiles/udt.dir/src/core.cpp.o.d -o CMakeFiles/udt.dir/src/core.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/core.cpp

CMakeFiles/udt.dir/src/core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/core.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/core.cpp > CMakeFiles/udt.dir/src/core.cpp.i

CMakeFiles/udt.dir/src/core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/core.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/core.cpp -o CMakeFiles/udt.dir/src/core.cpp.s

CMakeFiles/udt.dir/src/epoll.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/epoll.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/epoll.cpp
CMakeFiles/udt.dir/src/epoll.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/udt.dir/src/epoll.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/epoll.cpp.o -MF CMakeFiles/udt.dir/src/epoll.cpp.o.d -o CMakeFiles/udt.dir/src/epoll.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/epoll.cpp

CMakeFiles/udt.dir/src/epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/epoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/epoll.cpp > CMakeFiles/udt.dir/src/epoll.cpp.i

CMakeFiles/udt.dir/src/epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/epoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/epoll.cpp -o CMakeFiles/udt.dir/src/epoll.cpp.s

CMakeFiles/udt.dir/src/list.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/list.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/list.cpp
CMakeFiles/udt.dir/src/list.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/udt.dir/src/list.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/list.cpp.o -MF CMakeFiles/udt.dir/src/list.cpp.o.d -o CMakeFiles/udt.dir/src/list.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/list.cpp

CMakeFiles/udt.dir/src/list.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/list.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/list.cpp > CMakeFiles/udt.dir/src/list.cpp.i

CMakeFiles/udt.dir/src/list.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/list.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/list.cpp -o CMakeFiles/udt.dir/src/list.cpp.s

CMakeFiles/udt.dir/src/md5.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/md5.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/md5.cpp
CMakeFiles/udt.dir/src/md5.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/udt.dir/src/md5.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/md5.cpp.o -MF CMakeFiles/udt.dir/src/md5.cpp.o.d -o CMakeFiles/udt.dir/src/md5.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/md5.cpp

CMakeFiles/udt.dir/src/md5.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/md5.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/md5.cpp > CMakeFiles/udt.dir/src/md5.cpp.i

CMakeFiles/udt.dir/src/md5.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/md5.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/md5.cpp -o CMakeFiles/udt.dir/src/md5.cpp.s

CMakeFiles/udt.dir/src/packet.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/packet.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/packet.cpp
CMakeFiles/udt.dir/src/packet.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/udt.dir/src/packet.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/packet.cpp.o -MF CMakeFiles/udt.dir/src/packet.cpp.o.d -o CMakeFiles/udt.dir/src/packet.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/packet.cpp

CMakeFiles/udt.dir/src/packet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/packet.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/packet.cpp > CMakeFiles/udt.dir/src/packet.cpp.i

CMakeFiles/udt.dir/src/packet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/packet.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/packet.cpp -o CMakeFiles/udt.dir/src/packet.cpp.s

CMakeFiles/udt.dir/src/queue.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/queue.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/queue.cpp
CMakeFiles/udt.dir/src/queue.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/udt.dir/src/queue.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/queue.cpp.o -MF CMakeFiles/udt.dir/src/queue.cpp.o.d -o CMakeFiles/udt.dir/src/queue.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/queue.cpp

CMakeFiles/udt.dir/src/queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/queue.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/queue.cpp > CMakeFiles/udt.dir/src/queue.cpp.i

CMakeFiles/udt.dir/src/queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/queue.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/queue.cpp -o CMakeFiles/udt.dir/src/queue.cpp.s

CMakeFiles/udt.dir/src/udtCommon.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/udtCommon.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/udtCommon.cpp
CMakeFiles/udt.dir/src/udtCommon.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/udt.dir/src/udtCommon.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/udtCommon.cpp.o -MF CMakeFiles/udt.dir/src/udtCommon.cpp.o.d -o CMakeFiles/udt.dir/src/udtCommon.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/udtCommon.cpp

CMakeFiles/udt.dir/src/udtCommon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/udtCommon.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/udtCommon.cpp > CMakeFiles/udt.dir/src/udtCommon.cpp.i

CMakeFiles/udt.dir/src/udtCommon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/udtCommon.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/udtCommon.cpp -o CMakeFiles/udt.dir/src/udtCommon.cpp.s

CMakeFiles/udt.dir/src/window.cpp.o: CMakeFiles/udt.dir/flags.make
CMakeFiles/udt.dir/src/window.cpp.o: /root/autodl-tmp/linux_c++/UDT-master/src/window.cpp
CMakeFiles/udt.dir/src/window.cpp.o: CMakeFiles/udt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/udt.dir/src/window.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/udt.dir/src/window.cpp.o -MF CMakeFiles/udt.dir/src/window.cpp.o.d -o CMakeFiles/udt.dir/src/window.cpp.o -c /root/autodl-tmp/linux_c++/UDT-master/src/window.cpp

CMakeFiles/udt.dir/src/window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/udt.dir/src/window.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/autodl-tmp/linux_c++/UDT-master/src/window.cpp > CMakeFiles/udt.dir/src/window.cpp.i

CMakeFiles/udt.dir/src/window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/udt.dir/src/window.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/autodl-tmp/linux_c++/UDT-master/src/window.cpp -o CMakeFiles/udt.dir/src/window.cpp.s

# Object files for target udt
udt_OBJECTS = \
"CMakeFiles/udt.dir/src/api.cpp.o" \
"CMakeFiles/udt.dir/src/buffer.cpp.o" \
"CMakeFiles/udt.dir/src/cache.cpp.o" \
"CMakeFiles/udt.dir/src/ccc.cpp.o" \
"CMakeFiles/udt.dir/src/channel.cpp.o" \
"CMakeFiles/udt.dir/src/core.cpp.o" \
"CMakeFiles/udt.dir/src/epoll.cpp.o" \
"CMakeFiles/udt.dir/src/list.cpp.o" \
"CMakeFiles/udt.dir/src/md5.cpp.o" \
"CMakeFiles/udt.dir/src/packet.cpp.o" \
"CMakeFiles/udt.dir/src/queue.cpp.o" \
"CMakeFiles/udt.dir/src/udtCommon.cpp.o" \
"CMakeFiles/udt.dir/src/window.cpp.o"

# External object files for target udt
udt_EXTERNAL_OBJECTS =

libudt.a: CMakeFiles/udt.dir/src/api.cpp.o
libudt.a: CMakeFiles/udt.dir/src/buffer.cpp.o
libudt.a: CMakeFiles/udt.dir/src/cache.cpp.o
libudt.a: CMakeFiles/udt.dir/src/ccc.cpp.o
libudt.a: CMakeFiles/udt.dir/src/channel.cpp.o
libudt.a: CMakeFiles/udt.dir/src/core.cpp.o
libudt.a: CMakeFiles/udt.dir/src/epoll.cpp.o
libudt.a: CMakeFiles/udt.dir/src/list.cpp.o
libudt.a: CMakeFiles/udt.dir/src/md5.cpp.o
libudt.a: CMakeFiles/udt.dir/src/packet.cpp.o
libudt.a: CMakeFiles/udt.dir/src/queue.cpp.o
libudt.a: CMakeFiles/udt.dir/src/udtCommon.cpp.o
libudt.a: CMakeFiles/udt.dir/src/window.cpp.o
libudt.a: CMakeFiles/udt.dir/build.make
libudt.a: CMakeFiles/udt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX static library libudt.a"
	$(CMAKE_COMMAND) -P CMakeFiles/udt.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/udt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/udt.dir/build: libudt.a
.PHONY : CMakeFiles/udt.dir/build

CMakeFiles/udt.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/udt.dir/cmake_clean.cmake
.PHONY : CMakeFiles/udt.dir/clean

CMakeFiles/udt.dir/depend:
	cd /root/autodl-tmp/linux_c++/UDT-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/autodl-tmp/linux_c++/UDT-master /root/autodl-tmp/linux_c++/UDT-master /root/autodl-tmp/linux_c++/UDT-master/build /root/autodl-tmp/linux_c++/UDT-master/build /root/autodl-tmp/linux_c++/UDT-master/build/CMakeFiles/udt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/udt.dir/depend
