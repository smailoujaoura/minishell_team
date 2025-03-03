/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:50:59 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/03 16:03:58 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_exit_status(char *str, int *status)
{
	int	i;
	int	n;
	int	error;

	i = -1;
	error = 0;
	n = ft_atoi(str, &error);
	if (error)
	{
		printf("minishell: exit: %s: numeric argument required\n", str);
		*status = 2;
		exit(2);
	}
	*status = n % 256;
	exit(n % 256);
}

void	builtin_exit(char **argv, int *status)
{
	int	i;

	if (!argv[1])
	{
		*status = 0;
		exit(EXIT_SUCCESS);
	}
	i = -1;
	while (argv[1][++i])
	{
		if (argv[1][i] < 0 || argv[1][i] > 9)
		{
			printf("minishell: exit: %s: numeric argument required\n", argv[1]);
			*status = 2;
			exit(2);
		}
	}
	if (argv[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		*status = 1;
		return ;
	}
	check_exit_status(argv[1], status);
}
