/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:50:59 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/27 07:57:49 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    check_exit_status(char *str, int *status)
{
    int i;

    i = -1;
    while (str[++i])
    {
        if (str[i] < '0' && str[i] > 9)
        {
            printf("exit: %s: numeric argument required\n", str);
            *status = 2;
            exit(2);
        }
    }
}

void    builtin_exit(char **argv, int *status)
{
    int exit_status;

    write(1, "exit\n", 5);
    if (!argv[1])
    {
        *status = 0;
        exit(EXIT_SUCCESS);
    }
    if (argv[2])
    {
        write(2, "exit: too many arguments\n", 25);
        *status = 1;
        return ;
    }
    check_exit_status(argv[1], status);
    exit_status = ft_atoi(argv[1]);
    exit_status = exit_status % 256;
    *status = exit_status;
    exit(exit_status);
}
