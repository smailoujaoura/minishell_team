#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void f(void)
{
	int fd = open("test", O_RDWR | O_CREAT, 0644);
	printf("[%d]\n", fd);
	if (fd == 3)
	{
		printf("No fd leaks\n");
	}
	else
		printf("fds leaked: %d \n", fd - 2);
}

int main(void)
{
	atexit(f);
	// int fd = open("test", O_RDWR | O_CREAT, 0644);
	// printf("[%d]\n", fd);
}