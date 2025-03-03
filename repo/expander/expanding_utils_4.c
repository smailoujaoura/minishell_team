/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:06 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/03 09:19:22 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// checks if a two chars: current and one next to it may represent variable
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
		else
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
	(*i) += 1;
	while (str[*i] && ft_strchr(MID, str[*i]))
	{
		(*i)++;
	}
	key = ft_substr(&str[j], 0, *i - j, SOUJAOUR);
	return (get_value_wrapper(key, mini->env));
}
