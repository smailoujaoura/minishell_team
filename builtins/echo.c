/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:52:26 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/20 16:55:09 by soujaour         ###   ########.fr       */
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

int	ft_write(char **argv, int stop_check_opt, int *gl_stat, int i)
{
	if (stop_check_opt)
	{
		if (write(STDOUT_FILENO, argv[i], ft_strlen(argv[i])) == -1)
		{
			perror("minishell: echo: write error");
			*gl_stat = 1;
			return (1);
		}
	}
	if (stop_check_opt && argv[i + 1])
	{
		if (write(STDOUT_FILENO, " ", 1) == -1)
		{
			perror("minishell: echo: write error");
			*gl_stat = 1;
			return (1);
		}
	}
	return (0);
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
		if (ft_write(argv, stop_check_opt, gl_stat, i))
			return ;
		i++;
	}
	if (!option_n)
	{
		if (write(STDOUT_FILENO, "\n", 1) == -1)
		{
			perror("minishell: echo: write error");
			*gl_stat = 1;
			return ;
		}
	}
	*gl_stat = 0;
}
