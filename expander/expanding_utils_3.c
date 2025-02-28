/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:58 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/28 17:22:03 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*create_wilds(char *wilds)
{
	int		j;
	int		i;
	int		len;
	char	*new_wilds;

	len = 0;
	i = 0;
	while (wilds[i])
	{
		if (wilds[i] != QUOTE)
			len++;
		i++;
	}
	new_wilds = ft_calloc(len + 1, 1, SOUJAOUR);
	i = 0;
	j = 0;
	while (j < len)
	{
		if (wilds[i] != QUOTE)
			new_wilds[j++] = wilds[i];
		i++;
	}
	return (wilds);
}

int	inside_level_two(char *str, char *sources, int i)
{
	if (str[i] == '"' || str[i] == '\'')
	{
		while (sources[i] == FROM_VAR)
		{
			i++;
			if ((str[i] == '"' || str[i] == '\'') && sources[i] == FROM_VAR)
				return (1);
		}
	}
	return (0);
}

void	wild_shell_copy_helper(char *quotes, char *str, char *sources, int i)
{
	int	inside_lvl_one;
	int	inside_lvl_two;

	inside_lvl_one = 0;
	inside_lvl_two = 0;
	while (quotes[i])
	{
		if (!inside_lvl_one && quotes[i] == QUOTE)
			inside_lvl_one++;
		else if (inside_lvl_one && quotes[i] == QUOTE)
			inside_lvl_one--;

		if (!inside_lvl_two && inside_level_two(str, sources, i))
			inside_lvl_two++;
		else if (inside_lvl_two && (str[i] == '"' || str[i] == '\''))
			inside_lvl_two--;
			
		if (str[i] == '*' && !inside_lvl_one && sources[i] == LITERAL)
			quotes[i] = IS_WILD;
		else if (str[i] == '*' && !inside_lvl_two && !inside_lvl_one)
			quotes[i] = IS_WILD;
		else if (quotes[i] != SPLIT && quotes[i] != QUOTE)
			quotes[i] = NOT_WILD;
		i++;
	}
}

char	*wild_shell(char *sources, char *quotes, char *str)
{
	int		i;

	i = 0;
	while (sources[i])
	{
		if (sources[i] == SPLIT)
			quotes[i] = SPLIT;
		i++;
	}
	wild_shell_copy_helper(quotes, str, sources, 0);
	return (create_wilds(quotes));
}