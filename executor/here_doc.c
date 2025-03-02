/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:56:09 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/02 16:38:06 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define WARNA "minishell: warning: here-document at line"
#define WARNB "delimited by end-of-file (wanted `"

static int	g_heredoc;

void	report_sig_number(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	g_heredoc = -33;
	close(STDIN_FILENO);
}

char	*generate_random_name(void)
{
	char	*bytes;
	int		fd;
	int		i;

	bytes = ft_strdup("/tmp/.00000000000000000000", SOUJAOUR);
	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
		panic_exit("Open failed\n", 1337);
	i = read(fd, bytes + 6, 20);
	if (i == 0)
	{
		panic_exit("Read failed\n", 1337);
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

void	prompt_here_doc(const char *limiter, int fd, int num)
{
	char	*line;
	char	*save;

	line = NULL;
	while (g_heredoc != -33)
	{
		line = readline("> ");
		if (!line && g_heredoc != -33)
		{
			printf("%s %d %s%s')\n", WARNA, num, WARNB, limiter);
			break ;
		}
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, SIZE_MAX) == 0)
		{
			free(line);
			break ;
		}
		save = ft_strjoin(line, ft_strdup("\n", SOUJAOUR), SOUJAOUR);
		write(fd, save, ft_strlen(save));
		free(line);
	}
}

int	here_doc(t_chain *data, int num)
{
	char	*filename;
	int		fd1;
	int		fd2;

	g_heredoc = 0;
	filename = generate_random_name();
	fd1 = open(filename, O_WRONLY | O_CREAT, 0600);
	if (fd1 == -1)
		panic_exit("Open failed\n", 1338);
	fd2 = open(filename, O_RDONLY);
	if (fd2 == -1)
		panic_exit("Open failed\n", 7232);
	unlink(filename);
	setup_signals(4);
	prompt_here_doc(data->delim, fd1, num);
	setup_signals(3);
	setup_signals(5);
	close(fd1);
	data->fd = fd2;
	if (g_heredoc == -33)
	{
		close(data->fd);
		return (-1);
	}
	return (0);
}
