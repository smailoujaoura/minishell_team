/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:58:05 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/26 21:19:17 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void free_env(t_env *env_to_unset)
{
    free(env_to_unset->key);
    free(env_to_unset->value);
    free(env_to_unset->full);
    free(env_to_unset);
}

static void remove_and_rebuilt(t_env *env, char *arg)
{
    t_env *temp;

    temp = NULL;
    while (env)
    {
        temp = env->next;
        if (ft_strncmp(env->key, arg, ft_strlen(arg)) == 0)
        {
            free_env(env);
            env = temp;
            break ;
        }
        else if (ft_strncmp(temp->key, arg, ft_strlen(arg)) == 0)
        {
            if (temp->next)
                env->next = temp->next;
            else
                env->next = NULL;
            free_env(temp);
            break ;
        }
        env = env->next;
    }
}

// Unset an env var
void    builtin_unset(t_env *env, char **argv)
{
    int i;

    i = 0;
    if (!argv[1])
        return ;
    while (argv[++i])
    {
        if (!get_env_var(env, argv[i]))
            return ;
        remove_and_rebuilt(env, argv[i]);
    }
}
