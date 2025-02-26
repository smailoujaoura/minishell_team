/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:56:10 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/26 21:18:51 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Print env vars
void    builtin_env(t_env *env, char **argv)
{
    if (!env)
        return ;
    if (argv[1])
    {
        printf("env: '%s': No such file or directory\n", argv[1]);
        return ;
    }
    while (env)
    {
        printf("%s\n", env->full);
        env = env->next;
    }
}
