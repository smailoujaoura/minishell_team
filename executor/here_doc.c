/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:56:09 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/26 10:07:31 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*generate_random_name(void)
{
	char    		*bytes;
	int     		fd;
	int     		i;

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

void    prompt_here_doc(const char *limiter, int fd, int num)
{
	char    *line;

	line = NULL;
	while (1337)
	{
		line = readline("> ");
		if (!line)
			printf("minishell: warning: here-document at %d\n", num);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
}

void    here_doc(t_chain *data, int num)
{
	char	*filename;
	int		fd;

	filename = generate_random_name();
	fd = open(filename, O_RDWR | O_CREAT, 0600);
	if (fd == -1)
		panic_exit("Open failed\n", 1338);
	data->fd = fd;
	data->file = filename;
	prompt_here_doc(data->delim, fd, num);
}
