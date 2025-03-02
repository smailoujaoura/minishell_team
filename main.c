/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:21 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/02 16:53:25 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	minishell(t_shell *mini, struct termios *o, struct termios *i, int n)
{
	t_chain			*list;
	t_ast			*root;
	char			*line;

	list = NULL;
	while (1337)
	{
		n++;
		line = readline("Minishell: ");
		if (line == NULL)
			break ;
		root = parse_line(line, &list, &n);
		store_line(NULL, -1);
		setup_signals(2);
		executor(root, mini);
		setup_signals(3);
		free(line);
		list = NULL;
		ft_malloc(0, DEALLOCATE);
		if (tcsetattr(STDOUT_FILENO, TCSANOW, o) < 0
			|| tcsetattr(STDIN_FILENO, TCSANOW, i) < 0)
			return ;
	}
	ft_malloc_bkol(0, DEALLOCATE);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell			data;
	struct termios	tp_out;
	struct termios	tp_in;

	if (!isatty(STDIN_FILENO))
		return (1);
	if (isatty(STDOUT_FILENO) && tcgetattr(STDOUT_FILENO, &tp_out) < 0)
		return (1);
	if (tcgetattr(STDIN_FILENO, &tp_in) < 0)
		return (1);
	setup_signals(-1);
	data.env = handle_env(envp);
	data.last_exit = 0;
	minishell(&data, &tp_out, &tp_in, 0);
	(void)argv;
	(void)argc;
	return (0);
}
