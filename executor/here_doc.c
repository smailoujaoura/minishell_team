/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:56:09 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/13 14:42:10 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define WARNA "minishell: warning: here-document at line"
#define WARNB "delimited by end-of-file (wanted `"

char	*generate_random_name(void)
{
	char	*bytes;
	int		fd;
	int		i;

	bytes = ft_strdup("/tmp/.00000000000000000000", SOUJAOUR);
	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
		raise_error(strerror(errno));
	i = read(fd, bytes + 6, 20);
	if (i == 0)
	{
		raise_error(strerror(errno));
		close(fd);
	}
	close(fd);
	i = 6;
	while (i < 26)
	{
		bytes[i] = 'a' + bytes[i] % 26;
		if (!ft_isalnum(bytes[i]))
			bytes[i] = 'o';
		i++;
	}
	return (bytes);
}

void	return_status(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	exit(EXIT_FAILURE);
}

void	prompt_here_doc(const char *limiter, int fd, t_shell *mini)
{
	char	*line;
	char	*save;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("%s %d %s%s')\n", WARNA, mini->num, WARNB, limiter);
			break ;
		}
		if (ft_strncmp(line, limiter, SIZE_MAX) == 0)
		{
			free(line);
			break ;
		}
		save = ft_strjoin(line, ft_strdup("\n", SOUJAOUR), SOUJAOUR);
		write(fd, save, ft_strlen(save));
		free(line);
	}
	exit(EXIT_SUCCESS);
}

int	more_logic(t_shell *mini, t_chain *data, int fd1, int fd2)
{
	int	status;
	int	heredoc_proc;

	heredoc_proc = ft_fork();
	if (heredoc_proc == 0)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, return_status);
		close(fd2);
		prompt_here_doc(data->delim, fd1, mini);
	}
	else
		setup_signals(2);
	close(fd1);
	data->fd = fd2;
	wait(&status);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = WTERMSIG(status);
	return (status);
}

int	here_doc(t_chain *data, t_shell *mini)
{
	char	*filename;
	int		fd1;
	int		fd2;

	filename = generate_random_name();
	fd1 = open(filename, O_WRONLY | O_CREAT, 0600);
	if (fd1 == -1)
		raise_error(strerror(errno));
	fd2 = open(filename, O_RDONLY);
	if (fd2 == -1)
	{
		close(fd1);
		raise_error(strerror(errno));
	}
	unlink(filename);
	return (more_logic(mini, data, fd1, fd2));
}
