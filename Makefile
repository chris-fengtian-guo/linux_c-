# the compiler to use.
CC = g++

# compiler flags:
#  -Wall turns on most, but not all, compiler warnings
#  -std=c++11 to use the C++11 standard
CFLAGS  = -g  -Wall -std=c++11

# libraries to link into executable:
LIBS = -lboost_system -lrt

# the name to use for both the target source file, and the output file:
TARGET = shared_memory

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(LIBS)

clean:
	$(RM) $(TARGET)
