/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:05:20 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/14 12:56:42 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_lstsize_env(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		env = env->next;
		size++;
	}
	return (size);
}

t_env	*ft_lstlast_env(t_env *env)
{
	t_env	*temp;

	if (!env)
		return (NULL);
	temp = env;
	while (temp->next)
		temp = temp->next;
	return (temp);
}

// Function to add a new env var to the env linked list
void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	if (lst && *lst)
		ft_lstlast_env(*lst)->next = new;
	else
		*lst = new;
}

// Function to check if an env_var already existed
int	check_env(t_env *env, char *key)
{
	if (!env)
		return (0);
	while (env)
	{
		if (ft_strncmp(env->key, key, SIZE_MAX) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

char	*expand_env_var(t_env *env, char *exp_env)
{
	while (env)
	{
		if (ft_strncmp(env->key, exp_env, SIZE_MAX) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
