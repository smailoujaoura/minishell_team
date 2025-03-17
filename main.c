/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:21 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 08:43:04 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	g_sig_number;

void	statically_stored_shell(t_shell *mini, int action)
{
	static t_shell	*ptr;

	if (action == -1)
		ptr = mini;
	else if (action == -2)
	{
		ptr->flag = 1;
	}
	else
	{
		ptr->last_exit = g_sig_number + 128;
	}
}

void	third_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	close(STDIN_FILENO);
	g_sig_number = SIGINT;
	statically_stored_shell(NULL, SIGINT);
	statically_stored_shell(NULL, -2);
	setup_signals(1);
}

void	second_handler(int signum)
{
	(void)signum;
	g_sig_number = SIGINT;
	statically_stored_shell(NULL, SIGINT);
}

void	first_handler(int signum)
{
	(void)signum;
	g_sig_number = SIGINT;
	statically_stored_shell(NULL, SIGINT);
	handle_interrupt();
}

int	main(int argc, char *argv[], char *envp[])
{
	struct termios	initial;
	t_shell			data;

	if (!isatty(STDIN_FILENO))
		exit(1);
	if (!isatty(STDOUT_FILENO))
		rl_outstream = stderr;
	if (tcgetattr(STDIN_FILENO, &initial) < 0)
		return (1);
	init_shell(envp, &data);
	statically_stored_shell(&data, -1);
	minishell(&data, &initial);
	write(2, "exit\n", 6);
	return (data.last_exit);
	(void)argv;
	(void)argc;
}
