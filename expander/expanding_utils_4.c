/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:06 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 17:21:52 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define SETA " \t\n\v\f\r"

// checks if a two chars: current and one next to it may represent variable
int	is_var(char current, char next, char *set)
{
	if (current == '$')
	{
		if (ft_strchr(set, next) && next != 0)
		{
			if (next == '?')
				return (2);
			return (1);
		}
		else if (ft_strchr(SETA, next) || next == 0)
			return (3);
	}
	return (0);
}

// get variable value
char	*get_value_wrapper(char *var, t_env *env)
{
	t_env	*node;

	node = get_env_var(env, var);
	if (node == NULL)
		return (ft_strdup("", SOUJAOUR));
	else
		return (node->value);
}

// Get variable value or special variable value
char	*get_value(char *str, int *i, t_shell *mini, int type)
{
	int		j;
	char	*key;
	char	*value;

	if (type == 2)
	{
		key = ft_itoa(mini->last_exit);
		value = ft_strdup(key, SOUJAOUR);
		free(key);
		(*i) += 2;
		return (value);
	}
	else if (type == 3)
		return ((*i)++, ft_strdup("$", SOUJAOUR));
	j = *i + 1;
	(*i) += 1;
	while (str[*i] && ft_strchr(MID, str[*i]))
		(*i)++;
	key = ft_substr(&str[j], 0, *i - j, SOUJAOUR);
	value = get_value_wrapper(key, mini->env);
	if (!key[0] && mini->doubles)
		return (ft_strdup("$", SOUJAOUR));
	return (value);
}
