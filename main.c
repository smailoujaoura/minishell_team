/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:21 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/28 17:32:39 by soujaour         ###   ########.fr       */
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
	if (signum == SIGINT)
		handle_interrupt();
}

void	loop_minishell(t_shell *mini)
{
	t_chain	*list;
	t_ast	*root;
	char	*line;
	int		num;
	
	num = 0;
	list = NULL;
	while (1337)
	{
		num++;
		line = readline("Minishell: ");
		if (line == NULL)
			break ;
		root = parse_line(line, &list, &num);
		store_line(NULL, -1);
		executor(root, mini);
		free(line);
		list = NULL;
		ft_malloc(0, DEALLOCATE);
	}
	ft_malloc_bkol(0, DEALLOCATE);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell				data;
	struct sigaction	signals;

	sigfillset(&signals.sa_mask);
	data.env = handle_env(envp);
	if (!data.env)
		return (1);
	signals.sa_sigaction = handle_signals;
	sigaction(SIGINT, &signals, NULL);
	loop_minishell(&data);
	(void)argv;
	(void)argc;
	return (0);
}
