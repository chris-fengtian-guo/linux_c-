#include <stdio.h>
#include <unistd.h>

int main(void)
{
	execl("/bin/ls", "ls", "-l", "-a", NULL);

	return 0;
}

