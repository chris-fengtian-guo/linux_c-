#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>

int main() {
    
    mkfifo("/tmp/my_fifo", 0666);

    char buffer[256];
    int pipe = open("/tmp/my_fifo", O_RDONLY);

    while (true) {
        read(pipe, buffer, sizeof(buffer));
        std::cout << "Received: " << buffer << std::endl;
    }

    close(pipe);
    return 0;
}
