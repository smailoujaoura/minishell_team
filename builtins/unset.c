/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:58:05 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/05 21:05:05 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	remove_and_rebuilt(t_env *env, char *arg)
{
	t_env	*temp;

	temp = NULL;
	while (env)
	{
		temp = env->next;
		if (ft_strncmp(env->key, arg, SIZE_MAX) == 0)
		{
			env = temp;
			break ;
		}
		else if (ft_strncmp(temp->key, arg, SIZE_MAX) == 0)
		{
			if (temp->next)
				env->next = temp->next;
			else
				env->next = NULL;
			break ;
		}
		env = env->next;
	}
}

void	builtin_unset(t_env *env, char **argv)
{
	int	i;

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
