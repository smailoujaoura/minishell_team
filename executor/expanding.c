/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:36:59 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/27 17:10:16 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

# define MIDDLE "01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
# define START "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_?"

char	*get_value_wrapper(char *var, t_env *env)
{
	t_env	*node;

	node = get_env_var(env, var);
	if (node == NULL)
		return (ft_strdup("", SOUJAOUR)); // MAYBE THIS IS WHERE THE TRICK FOR THOSE ALL SEGFAULS
	else
		return (node->value);
}

int	is_var(char current, char next, char *set)
{
	if (current == '$')
	{
		if (ft_strchr(START, next))
		{
			if (next == '?')
				return (2);
			return (1);
		}
	}
	return (0);
}

char	*get_value(char *str, int *i, t_shell *mini)
{
	char	*key;
	int		type;
	int		j;

	type = is_var('$', str[*i + 1], START);
	if (type == 2)
	{
		return (ft_itoa(mini->last_exit));
	}
	j = *i + 1;
	(*i) += 2;
	while (is_var('$', str[*i + 1], MIDDLE))
	{
		(*i)++;
	}
	key = ft_substr(&str[j], j, *i - j, SOUJAOUR);
	return (get_value_wrapper(key, mini->env));
}

char	*expand_var(char *str, char **flags, t_shell *mini)
{
	int		i;
	int		type;
	char	*value;
	char	*result;
	int		should;

	i = 0;
	result = NULL;
	while (str[i])
	{
		if (is_var(str[i], str[i + 1], START))
		{
			value = get_value(str, &i, mini);
			result = ft_strjoin(result, value, SOUJAOUR);
			make_flag();
		}
		else
		{
			value = copy_literal(str, &i);
			result = ft_strjoin(result, value, SOUJAOUR);
			make_flag();
		}
	}
	return (result);
}

char	**expand_cmd(t_chain *cmd, t_shell *mini)
{
	char	**result;
	char	*flags;

	result = NULL;
	flags = NULL;
	char *expanded = expand_var(cmd->content, &flags, mini);
}