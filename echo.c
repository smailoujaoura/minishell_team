#include "minishell.h"

int	skip_rest(char *arg, int *new_line)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != 'n')
		{
			break ;
			return (1);
		}
	}
	*new_line = 0;
	return (0);
}

void	print_status(char **argv, int *status)
{
	if (argv[1] && ft_strlen(argv[1]) == 2 && !ft_strncmp(argv[1], "$?", 2))
	{
		printf("%d\n", *status);
		return (*status = 0);
	}
}

void	builtin_echo(char *cmd, char **argv, t_env *env, int *status)
{
	int	new_line;
	int	i;

	new_line = 1;
	i = 1;
	if (argv[1] == NULL)
		return ;
	print_status(argv, status);
	while (argv[i])
	{
		if (argv[i][0] == '-' && argv[i][1] == 'n')
			skip_rest(new_line, &new_line);
		else
			break ;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i++]);
		if (argv[i] != NULL)
			printf(" ");
	}
	if (new_line)
		printf("\n");
}