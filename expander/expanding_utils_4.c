/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:06 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/28 17:20:27 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_var(char current, char next, char *set)
{
	if (current == '$')
	{
		if (ft_strchr(set, next))
		{
			if (next == '?')
				return (2);
			return (1);
		}
	}
	return (0);
}

char	*get_value_wrapper(char *var, t_env *env)
{
	t_env	*node;

	node = get_env_var(env, var);
	if (node == NULL)
		return (ft_strdup("", SOUJAOUR));
	else
		return (node->value);
}

char	*get_value(char *str, int *i, t_shell *mini)
{
	char	*key;
	char	*value;
	int		type;
	int		j;

	type = is_var('$', str[*i + 1], STRT);
	if (type == 2)
	{
		key = ft_itoa(mini->last_exit);
		value = ft_strdup(key, SOUJAOUR);
		free(key);
		(*i) += 2;
		return (value);
	}
	j = *i + 1;
	(*i) += 2;
	while (str[*i] && is_var('$', str[*i], MID))
	{
		(*i)++;
	}
	key = ft_substr(&str[j], 0, *i - j, SOUJAOUR);
	return (get_value_wrapper(key, mini->env));
}
