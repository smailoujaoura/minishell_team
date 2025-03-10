/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 09:00:58 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/10 12:14:23 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// go into new line and replace line waiting to be sent with Enter key
// 		with nothing and then redisplay the prompt on the newline
void	handle_interrupt(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signals(int action)
{
	static struct sigaction	interactive;

	signal(SIGQUIT, SIG_IGN);
	if (action == 1)
	{
		sigfillset(&interactive.sa_mask);
		interactive.sa_flags = SA_RESTART;
		interactive.sa_sigaction = handler;
		sigaction(SIGINT, &interactive, NULL);
	}
	else if (action == 2)
	{
		sigfillset(&interactive.sa_mask);
		interactive.sa_flags = SA_RESTART;
		interactive.sa_sigaction = second_handler;
		sigaction(SIGINT, &interactive, NULL);
	}
	else if (action == 3)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
}
