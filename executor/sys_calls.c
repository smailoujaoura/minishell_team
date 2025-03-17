/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_calls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 11:35:53 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 09:58:36 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(int status)
{
	ft_malloc_bkol(0, DEALLOCATE);
	ft_malloc(0, DEALLOCATE);
	exit(status);
}

int	ft_pipe(int *pair)
{
	if (pipe(pair) == -1)
	{
		raise_error(strerror(errno));
	}
	return (0);
}

int	ft_dup2(int old, int new)
{
	if (dup2(old, new) == -1)
	{
		raise_error(strerror(errno));
	}
	return (new);
}

int	ft_dup(int old)
{
	int	new;

	new = dup(old);
	if (new == -1)
	{
		close(old);
		raise_error(strerror(errno));
	}
	return (new);
}

int	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		kill(0, SIGTERM);
		raise_error(strerror(errno));
		return (-1);
	}
	return (pid);
}
