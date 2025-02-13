/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epander.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 09:48:26 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/13 11:39:13 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_var(char *str, int f)
{
	int	i;
	int	pos;
	int	end;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	if (str[i] == '$')
		pos = i;
	i++;
	while (str[i] && str[i] != '$' && str[i] != '*')
		i++;
	end = i;
	if (f)
		return (end - pos);
	return (pos + 1);
}

char	*expand(t_ast *tree, t_env *env)
{
	char	*joined;
	char	*cmd;
	char	*var;
	t_env	*ret;

	if (tree->data->dollar)
	{
		var = ft_substr(tree->data->content, get_var(tree->data->content, 0),
			get_var(tree->data->content, 1), SOUJAOUR);
		ret = get_env_var(env, var);
	}
}