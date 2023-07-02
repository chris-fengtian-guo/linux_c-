# the compiler to use.
CC = g++

# compiler flags:
#  -Wall turns on most, but not all, compiler warnings
#  -std=c++11 to use the C++11 standard
CFLAGS  = -g  -Wall -std=c++11
CFLAGS  = -g  -Wall -std=c++11 -pthread

# libraries to link into executable:
LIBS = -lboost_system -lrt
LIBS = -lboost_system -lrt -pthread -lglog

# the name to use for both the target source file, and the output file:
TARGET1 = shared_memory
TARGET2 = thread_test
TARGET3 = google_log


all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(TARGET1).cpp
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).cpp $(LIBS)
$(TARGET2): $(TARGET2).cpp
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).cpp $(LIBS)
$(TARGET3): $(TARGET3).cpp
	$(CC) $(CFLAGS) -o $(TARGET3) $(TARGET3).cpp $(LIBS)



clean:
	$(RM) $(TARGET1) $(TARGET2)
