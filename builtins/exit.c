/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:50:59 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/17 21:23:54 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_exit(char **argv, int *status, t_shell *mini)
{
	static int	error = 0;
	int			num;

	if (!argv[1])
		exit(mini->last_exit);
	else if (argv[1])
	{
		num = ft_atoi(argv[1], &error);
		if (error)
		{
			write(2, "minishell: exit: ", 18);
			write(2, argv[1], ft_strlen(argv[1]));
			write(2, ": numeric argument required\n", 29);
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
