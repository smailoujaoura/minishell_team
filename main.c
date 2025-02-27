/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:21 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/27 10:10:57 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	handle_interrupt(void)
{
	write(2, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_signals(int signum, siginfo_t *info, void *ptr)
{
	(void)info;
	(void)ptr;
	printf("here\n");
	sleep(1);
	if (signum == SIGINT)
		handle_interrupt();
}

void	exit_shell(void)
{
	exit(0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell				data;
	struct sigaction	signals;

	sigfillset(&signals.sa_mask);

	data.env = handle_env(envp);
	signals.sa_sigaction = handle_signals;
	loop_minishell(&data);
	
	(void)argc;
	(void)argv;

}
