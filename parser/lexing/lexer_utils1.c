/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:38 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/02 19:51:49 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// skips and increment all the string inside quotes
// stoping at the character after the closing quote
void	handle_quotes(char **start, char target)
{
	int	quotes_flag;

	quotes_flag = 0;
	while (**start)
	{
		if (quotes_flag && **start == target)
			quotes_flag = 0;
		else if (!quotes_flag && **start == target)
			quotes_flag = 1;
		else if (!quotes_flag)
			break ;
		(*start)++;
	}
}

// will skip ONCE or TWICE depending on whether >> or >
// skips also twice for <<, and once for <
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
