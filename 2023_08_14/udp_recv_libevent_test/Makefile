# The compiler to use
CC = g++

# Compiler flags:
# -g for debugging info in executable
# -Wall to turn on most compiler warnings
# -std=c++11 to use the C++11 standard
#CFLAGS = -v -g -Wall -std=c++11 -pthread
CFLAGS = -g -Wall -std=c++11 -pthread

CXXFLAGS = -I/root/autodl-tmp/linux_c++_clean/my_libs/include
LDFLAGS = -L/root/autodl-tmp/linux_c++_clean/my_libs/lib -levent

# Libraries to link into executable
LIBS = -levent -luv -lrt -lglog -pthread
# libraries to link into executable:
#LIBS = -lboost_system -lrt -lglog -pthread -lcrypto

# Create a list of source files
SOURCES = $(wildcard *.cpp)

# Create a list of executables
TARGETS = $(SOURCES:.cpp=)

# Default rule
all: $(TARGETS)

udp_recv_libevent: udp_recv_libevent.cpp
	    $(CXX) $(CXXFLAGS) udp_recv_libevent.cpp $(LDFLAGS) -o udp_recv_libevent

# Rule to make each target
%: %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Clean up the build
.PHONY: clean
clean:
	rm -f $(TARGETS)
