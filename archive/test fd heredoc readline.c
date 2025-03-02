#include "includes/minishell.h"

static int g_sig;

void	ptr(int signum)
{
	int		saved_fd;
	g_sig = -1;
	saved_fd = dup(STDIN_FILENO);
	close(STDIN_FILENO);
	// dup(saved_fd);
}

int main(void)
{
	char	*line;
	signal(SIGINT, ptr);
	// pid_t id = fork();

	// if (id == 0)
	// {
		// signal(SIGINT, SIG_DFL);
	while (1)
	{
		if (g_sig == -1)
			break ;
		line = readline("readline: ");
		if (line == NULL)
			break ;
	}
	// }
	// wait(NULL);
	printf("Now I quit\n");
	while(1)
	{
		printf("yes \n");
		sleep(1);
	}
}
