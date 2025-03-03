/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:41:27 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/03 14:27:28 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_pipe(t_chain *prev, t_chain *next)
{
	if (next == NULL)
		return (0);
	if (next->type == PIPE || next->type == AND || next->type == OR || !prev)
	{
		if (!next)
			printf("%s '%s'\n", SYNTAXERR, "newline");
		else if (!prev)
			printf("%s `|'\n", SYNTAXERR);
		else
			printf("%s ```%s'\n", SYNTAXERR, next->content);
		return (1);
	}
	return (0);
}

int	check_logicals(t_chain *prev, t_chain *next)
{
	if (next == NULL)
		return (0);
	if (next->type == PIPE || next->type == AND || next->type == OR)
	{
		if (!next)
			printf("%s `%s'\n", SYNTAXERR, "newline");
		else
			printf("%s `%s'\n", SYNTAXERR, next->content);
		return (1);
	}
	(void)prev;
	return (0);
}

int	check_redirs(t_chain *prev, t_chain *next)
{
	if (!next || next->type != WORD)
	{
		if (!next)
			printf("%s `%s'\n", SYNTAXERR, "newline");
		else
			printf("%s `%s'\n", SYNTAXERR, next->content);
		return (1);
	}
	(void)prev;
	return (0);
}

int	check_paren(t_chain *prev, t_chain *next, int paren)
{
	if (!next)
		return (0);
	if (paren == L_PAREN)
	{
		if (next->type == R_PAREN || next->type == AND
			|| next->type == OR || next->type == PIPE)
		{
			printf("%s `%s'\n", SYNTAXERR, next->content);
			return (1);
		}
	}
	if (paren == R_PAREN)
	{
		if (next->type == L_PAREN)
		{
			printf("%s `%s'\n", SYNTAXERR, next->content);
			return (1);
		}
	}
	(void)prev;
	return (0);
}
