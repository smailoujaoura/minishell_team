/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:38 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/13 11:11:45 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_quotes(char **start, char target, char opposite)
{
	int i;

	i = 0;
	while (**start)
	{
		if (i && **start == target)
			i = 0;
		else if (!i && **start == target)
			i = 1;
		else if (!i && (ft_strchr(WHITESPACE, **start) || ft_strchr(SYMBOLS, **start) || **start == opposite))
			break ;
		(*start)++;
	}
}

void	handle_redirs(char **start)
{
	if (**start == '<') 
	{
		(*start)++;
		if (**start == '<')
			(*start)++;
	}
	else if (**start == '>')
	{
		(*start)++;
		if (**start == '>')
			(*start)++;
	}
}

int	is_wildcard(char *str)
{
	int	i;
	int	d;
	int	s;

	i = 0;
	d = 0;
	s = 0;
	while (str[i])
	{
		if (d && str[i] == '"' && !s)
			d = 0;
		else if (!d && str[i] == '"' && !s)
			d = 1;
		if (s && str[i] == '\'' && !d)
			s = 0;
		else if (!s && str[i] == '\'' && !d)
			s = 1;
		if (!s && !d && str[i] == '*')
			return (i + 1);
		i++;
	}
	return (0);
}

int	handle_dollar(char *str)
{
	int	i;
	int	d;
	int	s;
	int	n;

	i = 0;
	d = 0;
	s = 0;
	n = 0;
	while (str[i])
	{
		if (d && str[i] == '"' && !s)
			d = 0;
		else if (!d && str[i] == '"' && !s)
			d = 1;
		if (s && str[i] == '\'' && !d)
			s = 0;
		else if (!s && str[i] == '\'' && !d)
			s = 1;
		if (!s && !d && str[i] == '$')
			n++;
		else if (!s && d && str[i] == '$')
			n++;
		i++;
	}
	if (n)
		return (n);
	return (0);
}

void	find_type(t_chain *list)
{
	list->type = WORD;
	if (ft_strchr(list->content, '*') && is_wildcard(list->content))
	{
		list->wildcard = WILDCARD;
		if (handle_dollar(list->content))
			list->dollar = DOLLAR;
	}
	else if (*list->content == '"' || *list->content == '\'')
	{
		if (handle_dollar(list->content))
			list->dollar = DOLLAR;
	}
	else if (list->content[0] == '$' || handle_dollar(list->content))
	{
		list->dollar = DOLLAR;
	}
}
