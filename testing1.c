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
		*status = 0;
	}
}

void	builtin_echo(char **argv, int *status, int out_fd)
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
			skip_rest(argv[i], &new_line);
		else
			break ;
		i++;
	}
	while (argv[i])
	{
		write(out_fd, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1] != NULL)
			write(out_fd, " ", 1);
		i++;
	}
	if (new_line)
		write(out_fd, "\n", 1);
}

int main(void)
{
	int fd_pair[2];
	if (pipe(fd_pair) != 0)
		printf("Pipe error\n");

	char *argv[5] = {"ehco", "-n", "1234567890", "abcd", 0};
	int status = 27;

	builtin_echo(argv, &status, fd_pair[1]);

	close(fd_pair[1]);
	char buf[10];
	printf("Printing from pipe: \n");
	while (1)
	{
		int read_size = read(fd_pair[0], buf, 9);
		if (read_size == 0)
			break ;
		buf[read_size] = 0;
		printf("[%s]", buf);
	}
	close(fd_pair[0]);
}