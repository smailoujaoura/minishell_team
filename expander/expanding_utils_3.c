/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:58 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/03 12:11:33 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// create the string containing markings of wildcards positions
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

// mark if the char at position x is a wild if it come outside quotes
void	wild_shell_copy_helper(char *quotes, char *str, char *sources, int i)
{
	int	flags;

	flags = 0;
	while (quotes[i])
	{
		if (!flags && quotes[i] == QUOTE)
			flags++;
		else if (flags && quotes[i] == QUOTE)
			flags--;
		if (str[i] == '*' && !flags && sources[i] == LITERAL)
			quotes[i] = IS_WILD;
		else if (quotes[i] != SPLIT && quotes[i] != QUOTE)
			quotes[i] = NOT_WILD;
		i++;
	}
}

// build the flags for wildcards
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
