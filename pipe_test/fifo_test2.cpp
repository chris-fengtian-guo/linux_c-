#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

int main() {
    
    int pipe = open("/tmp/my_fifo", O_RDONLY);
    char buffer[256];

    while (true) {
        
        ssize_t size = read(pipe, buffer, sizeof(buffer)-1);

        if (size <= 0) {
            
            break;
        }

        
        buffer[size] = '\0';

        
        std::cout << buffer << std::endl;
    }

    
    close(pipe);

    return 0;
}
