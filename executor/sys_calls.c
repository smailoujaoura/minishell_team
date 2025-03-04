/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_calls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 11:35:53 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/04 13:52:45 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pipe(int *pair)
{
	if (pipe(pair) == -1)
	{
		panic_exit("pipe failed\n", 0);
	}
	return (0);
}

int ft_dup2(int old, int new)
{
	if (dup2(old, new) == -1)
	{
		// close(old);
		panic_exit("dup2 failed", 0);
	}
	// close(old);
	return (new);
}

int ft_dup(int old)
{
	int	new;

	new = dup(old);
	if (new == -1)
	{
		close(old);
		panic_exit("dup failed", 0);
	}
	return (new);
}

int ft_fork(void)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		kill(0, SIGTERM); // might need to use sigkill afterwards if they don't die may sleep 1 and then sigkill
		panic_exit("fork failed", 0);
		return (-1);
	}
	return (pid);
}
