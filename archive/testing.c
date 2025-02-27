#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t child = fork();

	if (child == 0)
	{
		printf("I am the child\n");
	}
	if (child == -1)
	{
		perror("Fork failure");
		return (1);
	}
	else
		printf("I am the parent\n");
}
