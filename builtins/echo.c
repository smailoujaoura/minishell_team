/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:52:26 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/06 14:07:23 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	check_option(char *argv, int *option_n, int *stop_check_opt)
{
	int	i;

	i = 2;
	if (argv[0] == '-' && argv[1] == 'n' && !*stop_check_opt)
	{
		while (argv[i] && argv[i] == 'n')
			i++;
		if (argv[i] == '\0')
			(*option_n)++;
		else
			*stop_check_opt = 1;
	}
	else
		*stop_check_opt = 1;
}

void	builtin_echo(char **argv, int *gl_stat)
{
	int	option_n;
	int	i;
	int	stop_check_opt;

	i = 1;
	option_n = 0;
	stop_check_opt = 0;
	while (argv[i])
	{
		check_option(argv[i], &option_n, &stop_check_opt);
		if (stop_check_opt)
			write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (stop_check_opt && argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!option_n)
		write(STDOUT_FILENO, "\n", 1);
	*gl_stat = 0;
}
