#include "minishell.h"

void	bkolani(char *argv[], char *envp[]);
void	loop_minishell(void);

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;

	bkolani(argv, envp);
	loop_minishell();
}