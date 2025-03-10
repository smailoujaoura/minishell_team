/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:21 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/10 14:13:57 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	g_sig_number;

void	statically_stored_shell(t_shell *mini, int action)
{
	static t_shell	*ptr;

	if (action == -1)
		ptr = mini;
	else
	{
		ptr->last_exit = g_sig_number + 128;
	}
}

void	second_handler(int signum, siginfo_t *info, void *ptr)
{
	g_sig_number = SIGINT;
	statically_stored_shell(NULL, SIGINT);
	(void)info;
	(void)ptr;
	(void)signum;
}

void	handler(int signum, siginfo_t *info, void *ptr)
{
	g_sig_number = SIGINT;
	handle_interrupt();
	statically_stored_shell(NULL, SIGINT);
	(void)info;
	(void)ptr;
	(void)signum;
}

void	minishell(t_shell *mini, struct termios *initial)
{
	t_chain			*list;
	t_ast			*root;
	char			*line;
	int				num;

	num = 1;
	while (1337)
	{
		list = NULL;
		setup_signals(1);
		line = readline("Minishell:$ ");
		if (line == NULL)
			break ;
		root = parse_line(line, &list, &num, mini);
		free(line);
		setup_signals(2);
		executor(root, mini);
		ft_malloc(0, DEALLOCATE);
		num++;
		mini->volatile_exit = 0;
		if (tcsetattr(STDIN_FILENO, TCSANOW, initial) < 0)
			return ;
	}
	ft_malloc_bkol(0, DEALLOCATE);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell			data;
	struct termios	initial;

	if (!isatty(STDIN_FILENO))
		return (1);
	if (tcgetattr(STDIN_FILENO, &initial) < 0)
		return (1);
	envp = make_env(envp);
	data.env = handle_env(envp);
	data.last_exit = 0;
	statically_stored_shell(&data, -1);
	minishell(&data, &initial);
	return (data.last_exit);
	(void)argv;
	(void)argc;
}
