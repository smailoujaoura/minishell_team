/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 09:00:58 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/03 12:59:36 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// setup signals for the child as they are not default because of inheritence
// of the signals despositions from the parent process for SIGINT, SIGQUIT
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// go into new line and replace line waiting to be sent with Enter key
// 		with nothing and then redisplay the prompt on the newline
void	handle_interrupt(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// handler 
void	handler(int signum, siginfo_t *info, void *ptr)
{
	(void)info;
	(void)ptr;
	if (signum == SIGINT)
		handle_interrupt();
}

/*
will setup signals based on a few circumstances:
	action == -1: Initializes the structs and for interactive mode:
		^+C(SIGINT) or ^+\(SIGQUIT) do not result process termination
	action == 2: for when the parent is waiting for child to execute
	action == 3: restores initial state (interactive mode) for SIGINT
				does not restore it for SIGQUIT because it was 
				never changes
	action == 4: handle heredoc signals before opening heredoc
	action == 5: restore signals after heredoc
*/
void	handle_here_doc_signal(int action)
{
	static int	saved_fd;

	if (action == 4)
	{
		signal(SIGINT, report_sig_number);
		saved_fd = dup(STDIN_FILENO);
		if (saved_fd < 0)
		{
			perror("minishell");
			return ;
		}
	}
	else if (action == 5)
	{
		if (dup2(saved_fd, STDIN_FILENO) < 0)
		{
			perror("minishell");
			return ;
		}
		close(saved_fd);
	}
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
		signal(SIGINT, SIG_IGN);
	else if (action == 3)
		sigaction(SIGINT, &interactive, NULL);
	else
		handle_here_doc_signal(action);
}
