#include "includes/minishell.h"
#include <sys/ioctl.h>

int	g_sig = 0;

void	handler(int signum)
{
	// ioctl();
	// rl_done = 1;
	g_sig = 130;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

int main(void)
{
	signal(SIGINT, handler);

	while (true && g_sig != 130)
	{
		char * line = readline("> ");
		if (line == NULL)
			break ;
		printf("line:[%s]\n", line);
	}
	printf("g_sig: %d\n", g_sig);
}