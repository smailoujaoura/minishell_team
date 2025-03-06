/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:51:08 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 20:05:46 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// count len of new str execluding removable characters marked by flags
int	count_len(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] != REMOV)
			len++;
		i++;
	}
	return (len);
}

// copy excluding removables like quotes
void	copy_helper(char *str, char *flags, char *new_str, char *new_flg)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (flags[i])
	{
		if (flags[i] != REMOV)
		{
			new_str[j] = str[i];
			new_flg[j] = flags[i];
			j++;
		}
		i++;
	}
}

// mark what needs to be removed in flag and str
char	*remove_flags(char *flags, char **actual, char *str)
{
	char	*new_flg;
	char	*new_str;
	int		i;

	i = 0;
	while (flags[i])
	{
		if (flags[i] != QUOTE)
			flags[i] = flags[i];
		else if (flags[i] == QUOTE && flags[i + 1] == QUOTE
			&& (flags[i + 2] == SPLIT || !flags[i + 2])
			&& (i == 0 || flags[i - 1] == SPLIT || find_split(flags, i - 1)))
			flags[i] = HANDLE;
		else
			flags[i] = REMOV;
		i++;
	}
	new_flg = ft_calloc(count_len(flags) + 1, 1, SOUJAOUR);
	new_str = ft_calloc(count_len(flags) + 1, 1, SOUJAOUR);
	copy_helper(str, flags, new_str, new_flg);
	*actual = new_str;
	return (new_flg);
}

// assign quotes positions into flags
void	assign_flag(char *edit, char which, int one, int two)
{
	if (which == '"' && one == 0)
		*edit = QUOTE;
	else if (which == '\'' && two == 0)
		*edit = QUOTE;
	else
		*edit = NOT_QUOTE;
}

// manipulates the flag and str(command, file, arg, wildcard, ...)
// building building the full flags
char	*ultimate(char *str, char *flags, int one, int two)
{
	char	*quotes_flag;
	char	*ultimate;
	int		i;

	quotes_flag = ft_calloc(ft_strlen(str) + 1, 1, SOUJAOUR);
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' && one != 1 && flags[i] == LITERAL)
			two++;
		if (str[i] == '\'' && two != 1 && flags[i] == LITERAL)
			one++;
		if (two == 2)
			two = 0;
		if (one == 2)
			one = 0;
		if (flags[i] == LITERAL && (str[i] == '"' || str[i] == '\''))
			assign_flag(&quotes_flag[i], str[i], one, two);
		else
			quotes_flag[i] = NOT_QUOTE;
		i++;
	}
	ultimate = wild_shell(flags, quotes_flag, str);
	return (ultimate);
}
