/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 09:00:58 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/02 11:18:49 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	handle_interrupt(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handler(int signum, siginfo_t *info, void *ptr)
{
	(void)info;
	(void)ptr;
	if (signum == SIGINT)
		handle_interrupt();
}

void	setup_signals(int action)
{
	static struct sigaction	interactive;

	if (action == -1)
	{
		sigfillset(&interactive.sa_mask);
		interactive.sa_flags = SA_SIGINFO | SA_RESTART;
		interactive.sa_sigaction = handler;
		sigaction(SIGINT, &interactive, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (action == 2)
	{
		signal(SIGINT, SIG_IGN);
	}
	else if (action == 3)
	{
		sigaction(SIGINT, &interactive, NULL);
	}
}
