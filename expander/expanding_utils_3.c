/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:58 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 20:30:29 by soujaour         ###   ########.fr       */
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
void	wild_shell_copy_helper(char *quotes, char *str, int i)
{
	int	flags;

	flags = 0;
	while (quotes[i])
	{
		if (!flags && quotes[i] == QUOTE)
			flags++;
		else if (flags && quotes[i] == QUOTE)
			flags--;
		if (str[i] == '*' && !flags)
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
	wild_shell_copy_helper(quotes, str, 0);
	return (create_wilds(quotes));
}

int	find_split(char *flags, int i)
{
	while (i > 0 && flags[i] == REMOV)
	{
		i--;
	}
	if (flags[i] == SPLIT)
		return (1);
	return (0);
}

// remove quotes off delimiter
void	strip_heredoc(t_chain *node, char *delim)
{
	if (ft_strchr(delim, '"') || ft_strchr(delim, '\''))
		node->delim_in_quotes = 1;
	node->delim = remove_occurences(delim, 0, 0, 0);
}
