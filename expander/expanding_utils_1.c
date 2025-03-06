/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:25:13 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 20:35:58 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// change togglers for single quotes and double quotes
void	singles_doubles(int *singles, int *doubles, char c)
{
	if (c == '\'' && !*singles && !*doubles)
	{
		*singles = 1;
	}
	else if (c == '\'' && *singles)
	{
		*singles = 0;
	}	
	if (c == '"' && !*doubles && !*singles)
	{
		*doubles = 1;
	}	
	else if (c == '"' && *doubles)
	{
		*doubles = 0;
	}
}

// the flags are just a way of marking what each part of string means:
// variable expansion or just copied (literl) or there is a delimiter
// the delimiter for splitting and also later on this flag will have wildcards
// these flags help in rmeoving quotes and expanding wildcards 
void	construct_flags(char *str, char flag, char **flags)
{
	char	*new_flag;
	size_t	len;
	size_t	i;

	len = ft_strlen(str);
	new_flag = ft_calloc(len + 1, sizeof(char), SOUJAOUR);
	i = 0;
	while (i < len)
	{
		new_flag[i] = flag;
		i++;
	}
	new_flag[i] = '\0';
	if (*flags == NULL)
		*flags = new_flag;
	else
		*flags = ft_strjoin(*flags, new_flag, SOUJAOUR);
}

int	var_to_var(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		i++;
	}
	i--;
	if (i > 0 && key[i] == '=')
		return (1);
	return (0);
}

// splits a variable contents based on spaces and joins but with flags
char	*handle_var_values(char *value, char *new, char **flags, int two)
{
	char	**var_values;
	int		i;

	i = 0;
	if (two || (new && var_to_var(new)))
	{
		new = ft_strjoin(new, value, SOUJAOUR);
		construct_flags(value, FROM_VAR, flags);
		return (new);
	}
	var_values = ft_split_custom(value, WHITESPACE, SOUJAOUR);
	while (var_values[i])
	{
		new = ft_strjoin(new, var_values[i], SOUJAOUR);
		construct_flags(var_values[i], FROM_VAR, flags);
		if (var_values[i + 1])
		{
			new = ft_strjoin(new, " ", SOUJAOUR);
			*flags = ft_strjoin(*flags, "s", SOUJAOUR);
		}
		i++;
	}
	return (new);
}

// copies a literal until finding another variable or end of string
char	*just_copy(char *str, int *i, int *singles, int *doubles)
{
	char	*new;
	int		len;

	len = 0;
	new = NULL;
	while (str[*i])
	{
		singles_doubles(singles, doubles, str[*i]);
		if (!singles && str[*i] == '$')
			break ;
		else
		{
			while (str[*i + len] && (*singles
					|| !is_var(str[*i + len], str[*i + len + 1], STRT)))
			{
				len++;
				singles_doubles(singles, doubles, str[*i + len]);
			}
			new = ft_substr(str, *i, len, SOUJAOUR);
			(*i) += len;
			return (new);
		}
	}
	return (new);
}
