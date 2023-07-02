# The compiler to use
CC = g++

# Compiler flags:
# -g for debugging info in executable
# -Wall to turn on most compiler warnings
# -std=c++11 to use the C++11 standard
CFLAGS = -g -Wall -std=c++11 -pthread

# Libraries to link into executable
LIBS = -lboost_system -lrt -lglog -pthread
# libraries to link into executable:
LIBS = -lboost_system -lrt -lglog -pthread -lcrypto

# Create a list of source files
SOURCES = $(wildcard *.cpp)

# Create a list of executables
TARGETS = $(SOURCES:.cpp=)

# Default rule
all: $(TARGETS)

# Rule to make each target
%: %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Clean up the build
.PHONY: clean
clean:
	rm -f $(TARGETS)
