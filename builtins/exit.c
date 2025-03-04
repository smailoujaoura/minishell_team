/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:50:59 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/04 11:53:31 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_exit(char **argv, int *status)
{
	int	error;
	int	num;

	error = 0;
	if (!argv[1])
		exit(*status = 0);
	else if (argv[1])
	{
		num = ft_atoi(argv[1], &error);
		if (error)
		{
			printf("minishell: exit: %s: numeric argument required\n", argv[1]);
			*status = 2;
			exit(2);
		}
		else if (argv[2])
		{
			write(2, "minishell: exit: too many arguments\n", 36);
			*status = 1;
			return ;
		}
		else
			exit(num % 256);
	}
}
