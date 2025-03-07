/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:09:05 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/07 11:39:56 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_new_env(t_env *env, t_env *new_env, const char *line, char *str)
{
	if (!str)
		new_env->value = ft_strdup("", BKOLANI);
	else
		new_env->value = ft_strdup(str, BKOLANI);
	new_env->full = ft_strdup(line, BKOLANI);
	new_env->next = NULL;
	ft_lstadd_back_env(&env, new_env);
}

void	add_new_env_with_plus(t_env *env, t_env *new_env, const char *str)
{
	char	*new_full;

	new_full = ft_strjoin(new_env->key, "=", BKOLANI);
	if (!str)
		new_env->full = ft_strdup(new_full, BKOLANI);
	else
	{
		new_env->value = ft_strdup(str, BKOLANI);
		new_env->full = ft_strjoin(new_full, new_env->value, BKOLANI);
	}
	new_env->next = NULL;
	ft_lstadd_back_env(&env, new_env);
}

void	update_env_trunc(t_env *env, t_env *new_env, const char *line,
	const char *str)
{
	t_env	*temp;

	temp = NULL;
	temp = get_env_var(env, new_env->key);
	temp->value = NULL;
	temp->full = NULL;
	if (!str)
		temp->value = ft_strdup("", BKOLANI);
	else
		temp->value = ft_strdup(str, BKOLANI);
	temp->full = ft_strdup(line, BKOLANI);
}

void	update_env_concat(t_env *env, t_env *new_env, const char *str)
{
	char	*new_full;
	t_env	*temp;
	char	*value;

	temp = get_env_var(env, new_env->key);
	value = ft_strdup(temp->value, BKOLANI);
	temp->full = NULL;
	temp->value = NULL;
	if (!str)
	{
		new_full = ft_strjoin(new_env->key, "=", BKOLANI);
		temp->full = ft_strdup(new_full, BKOLANI);
	}
	else
	{
		temp->value = ft_strjoin(value, str, BKOLANI);
		new_full = ft_strjoin(new_env->key, "=", BKOLANI);
		temp->full = ft_strjoin(new_full, temp->value, BKOLANI);
	}
}
