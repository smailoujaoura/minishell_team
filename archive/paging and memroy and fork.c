#include "includes/minishell.h"

void	child_routine(int *ptr)
{
	printf("Child: %d\n", *ptr);
	*ptr = 1337;
	sleep(3);
	printf("ptr: %p \t Child again: %d\n", ptr, *ptr);
	exit(0);
}

int	main(void)
{
	int	*ptr;

	ptr = malloc(sizeof(int));
	*ptr = 42;
	pid_t id = fork();
	printf("num: %d\n", *ptr);
	if (id == 0)
		child_routine(ptr);
	else
		printf("Parent: %d\n", *ptr);
	sleep(3);
	wait(NULL);
	printf("ptr: %p \t parent again: %d\n", ptr, *ptr);
}

// COW