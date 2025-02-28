/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:38 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/28 17:39:31 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes(char **start, char target, char opposite)
{
	int	i;

	i = 0;
	while (**start)
	{
		if (i && **start == target)
			i = 0;
		else if (!i && **start == target)
			i = 1;
		else if (!i && (ft_strchr(WHITESPACE, **start)
				|| ft_strchr(SYMBOLS, **start) || **start == opposite))
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
