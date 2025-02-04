/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:41:27 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/04 16:44:07 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define SYNTAXERR "Minishell: syntax error near unexpected token"
# define ERR "Minishell: syntax error near unexpected token '%s'\n"
// what 

int	check_pipe(t_chain *prev, t_chain *next)
{
	if (!next || next->type == PIPE || next->type == AND || next->type == OR || !prev)
	{
		if (!next)
			printf("%s '%s'\n", SYNTAXERR, "newline");
		else if (!prev)
			printf("%s `|'\n", SYNTAXERR);
		else
			printf("%s '%s'\n", SYNTAXERR, next->content);
		return (1);
	}
	return (0);
}

int	check_logicals(t_chain *prev, t_chain *next)
{
	if (!next || next->type == PIPE || next->type == AND || next->type == OR)
	{
		if (!next)
			printf("%s '%s'\n", SYNTAXERR, "newline");
		else
			printf("%s '%s'\n", SYNTAXERR, next->content);
		return (1);
	}
	(void)prev;
	return (0);
}


int	check_word(t_chain *prev, t_chain *next)
{
	if (!next || next->type == PIPE || next->type == AND || next->type == OR)
	{
		if (!next)
			printf("%s '%s'\n", SYNTAXERR, "newline");
		else
			printf("%s '%s'\n", SYNTAXERR, next->content);
		return (1);
	}
	(void)prev;
	return (0);
}

int	check_out_redir(t_chain *prev, t_chain *next)
{
	(void)next;
	(void)prev;
	return (0);
}

int	check_heredoc(t_chain *prev, t_chain *next)
{
	(void)next;
	(void)prev;
	return (0);
}

int	check_in_redir(t_chain *prev, t_chain *next)
{
	(void)next;
	(void)prev;
	return (0);
}

int	check_lparen(t_chain *prev, t_chain *next)
{
	(void)next;
	(void)prev;
	return (0);
}

int	check_rparen(t_chain *prev, t_chain *next)
{
	(void)next;
	(void)prev;
	return (0);
}

int	check_wildcard(t_chain *prev, t_chain *next)
{
	(void)next;
	(void)prev;
	return (0);
}

int	multiple_tokens(t_chain *prev, t_chain *token, t_chain *next)
{
	if (token->type == PIPE)
		return (check_pipe(prev, next));
	if (token->type == AND || token->type == OR)
		return (check_logicals(prev, next));
	if (token->type == WORD)
		return (check_word(prev, next));
	if (is_redir(token, OUT))
		return (check_out_redir(prev, next));
	if (token->type == HEREDOC)
		return (check_heredoc(prev, next));
	if (token->type == IN)
		return (check_in_redir(prev, next));
	if (token->type == L_PAREN)
		return (check_lparen(prev, next));
	if (token->type == R_PAREN)
		return (check_rparen(prev, next));
	if (token->type == WILDCARD)
		return (check_wildcard(prev, next));
	else
	{
		printf("In else why\n");
		return (-1);
	}
	return (0);
}

int	one_token(t_chain *list)
{
	(void)list;
	return (0);
}

void	throw_error(t_chain *list)
{
	// free
	(void)list;
}

int	check_syntax(t_chain *list)
{
	if (list && !list->next)
		one_token(list);
	else
	{
		while (list && list->next)
		{
			if (multiple_tokens(list, list->next, list->back))
				throw_error(list);
			list = list->next;
		}
	}
	return (0);
}