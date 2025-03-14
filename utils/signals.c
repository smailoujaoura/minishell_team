/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 09:00:58 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/14 13:48:13 by soujaour         ###   ########.fr       */
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
	static int	saved_fd;

	signal(SIGQUIT, SIG_IGN);
	if (action == 1)
		signal(SIGINT, first_handler);
	else if (action == 2)
		signal(SIGINT, second_handler);
	else if (action == 3)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	else if (action == 4)
	{
		saved_fd = ft_dup(STDIN_FILENO);
		signal(SIGINT, third_handler);
	}
	else if (action == 5)
	{
		ft_dup2(saved_fd, STDIN_FILENO);
		close(saved_fd);
	}
}

void	return_status(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	exit(EXIT_FAILURE);
}
