/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:50:59 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/03 16:27:35 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_exit_status(char **argv, int *status)
{
	int	i;
	int	n;
	int	error;

	i = 0;
	error = 0;
	n = ft_atoi(argv[1], &error);
	while (argv[i])
	{
		error = 0;
		n = ft_atoi(argv[1], &error);
		if (error)
		{
			printf("minishell: exit: %s: numeric argument required\n", argv[1]);
			*status = 2;
			exit(2);
		}
		i++;
	}
	if (i >= 1)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		*status = 1;
		return ;
	}
}

void	builtin_exit(char **argv, int *status)
{
	if (!argv[1])
	{
		*status = 0;
		exit(EXIT_SUCCESS);
	}
	check_exit_status(argv, status);
}
