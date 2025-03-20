/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:23 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/20 21:13:09 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*copy_if(char *new, char *str, char *flag)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (flag[i] == 1)
		{
			new[k] = str[i];
			k++;
		}
		i++;
	}
	new[k] = '\0';
	return (new);
}

char	*remove_occurences(char *str, int i, int one, int two)
{
	char	*flag;
	char	*new;
	int		removables;

	removables = 0;
	flag = ft_calloc(ft_strlen(str) + 1, sizeof(char), SOUJAOUR);
	while (str[i])
	{
		if (str[i] == '"' && one != 1)
			two++;
		else if (str[i] == '\'' && two != 1)
			one++;
		else
			flag[i] = 1;
		if (two == 2 || one == 2)
			removables += 2;
		if (two == 2)
			two = 0;
		if (one == 2)
			one = 0;
		i++;
	}
	new = ft_malloc(ft_strlen(str) - removables + 1, ALLOCATE);
	copy_if(new, str, flag);
	return (new);
}
