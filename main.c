/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:21 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/02 09:01:35 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	loop_minishell(t_shell *mini, struct termios *tp_out, struct termios *tp_in)
{
	t_chain			*list;
	t_ast			*root;
	char			*line;
	int				lines_num;

	lines_num = 0;
	list = NULL;
	while (1337)
	{
		lines_num++;
		line = readline("Minishell: ");
		if (line == NULL)
			break ;
		root = parse_line(line, &list, &lines_num);
		store_line(NULL, -1);
		executor(root, mini);
		free(line);
		list = NULL;
		ft_malloc(0, DEALLOCATE);
		if (tcsetattr(STDOUT_FILENO, TCSANOW, tp_out) < 0
			|| tcsetattr(STDIN_FILENO, TCSANOW, tp_in) < 0)
			return (1);
	}
	ft_malloc_bkol(0, DEALLOCATE);
	return (0);
}

// if (!data.env) 
// should setup some envs even if the program strted with env -i gnore
// return (1);
int	main(int argc, char *argv[], char *envp[])
{
	t_shell			data;
	struct termios	tp_out;
	struct termios	tp_in;

	if (!isatty(STDIN_FILENO))
		return (1);
	setup_signals();
	data.env = handle_env(envp);
	if (isatty(STDOUT_FILENO) && tcgetattr(STDOUT_FILENO, &tp_out) < 0)
		return (1);
	if (tcgetattr(STDIN_FILENO, &tp_in) < 0)
		return (1);
	loop_minishell(&data, &tp_out, &tp_in);
	(void)argv;
	(void)argc;
	return (0);
}
