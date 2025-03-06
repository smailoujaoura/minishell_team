/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:21 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 12:26:08 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int g_sig_number;

void	statically_stored_shell(t_shell *mini, int action)
{
	static t_shell *ptr;

	if (action == -1)
		ptr = mini;
	else
	{
		ptr->last_exit = g_sig_number + 128;
	}
}

// 
void	second_handler(int signum, siginfo_t *info, void *ptr)
{
	g_sig_number = SIGINT;
	statically_stored_shell(NULL, SIGINT);
	(void)info;
	(void)ptr;
	(void)signum;
}

// handler 
void	handler(int signum, siginfo_t *info, void *ptr)
{
	g_sig_number = SIGINT;
	handle_interrupt();
	statically_stored_shell(NULL, SIGINT);
	(void)info;
	(void)ptr;
	(void)signum;
}

// Reads a line and parses and executes and frees the parsing by-products
void	minishell(t_shell *mini, struct termios *o, struct termios *i, int n)
{
	t_chain			*list;
	t_ast			*root;
	char			*line;

	while (1337)
	{
		setup_signals(1);
		line = readline("Minishell: ");
		if (line == NULL)
			break ;
		list = NULL;
		root = parse_line(line, &list, &n, mini);
		store_line(NULL, -1);
		free(line);
		setup_signals(2);
		executor(root, mini);
		ft_malloc(0, DEALLOCATE);
		if (tcsetattr(STDOUT_FILENO, TCSANOW, o) < 0)
			return ;
		if (tcsetattr(STDIN_FILENO, TCSANOW, i) < 0)
			return ;
		n++;
		ft_malloc(0, DEALLOCATE);
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
	envp = make_env(envp);
	data.env = handle_env(envp);
	data.last_exit = 0;
	statically_stored_shell(&data, -1);
	minishell(&data, &tp_out, &tp_in, 1);
	(void)argv;
	(void)argc;
	return (0);
}
