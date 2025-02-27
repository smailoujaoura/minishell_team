/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:41:27 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/27 07:54:25 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

# define SYNTAXERR "Minishell: syntax error near unexpected token"
# define ERR "Minishell: syntax error near unexpected token '%s'\n"
// what 

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


int	check_word(t_chain *prev, t_chain *next)
{
	(void)prev;
	(void)next;
	return (0);
}

int	check_out_redir(t_chain *prev, t_chain *next)
{
	// or we in otherwords anything else after redir must be a word. 
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

int	check_heredoc(t_chain *prev, t_chain *next)
{
	// or we in otherwords anything else after redir must be a word. 
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

int	check_in_redir(t_chain *prev, t_chain *next)
{
	// or we in otherwords anything else after redir must be a word. 
	if (!next || next->type == PIPE || next->type == AND || next->type == OR || next->type == L_PAREN || next->type == R_PAREN)
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

int	check_lparen(t_chain *prev, t_chain *next)
{
	// need to check if we don't have closed paren.
	if (next->type == R_PAREN || next->type == AND || next->type == OR || next->type == PIPE)
	{
		printf("%s `%s'\n", SYNTAXERR, next->content);
		return (1);
	}
	(void)prev;
	return (0);
	return (0);
}

int	check_rparen(t_chain *prev, t_chain *next)
{
	// need to check if we don't have opened parenthesis. 
	if (next && next->type == L_PAREN)
	{
		printf("%s `%s'\n", SYNTAXERR, next->content);
		return (1);
	}
	if (next && next->type == R_PAREN)
	{
		printf("We don't do arithmetics in our tiny shell. Check 42sh for more...\n");
		return (1);
	}
	(void)prev;
	return (0);
}

int	check_wildcard(t_chain *prev, t_chain *next)
{
	// wild card for now can come anywhere.
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
	if (is_redir(token, IN))
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
	if (!(list->type == WORD || list->type == WILDCARD))
	{
		printf("%s `%s'\n", SYNTAXERR, list->content);
		return (1);
	}
	if (is_redir(list, IN + OR + OUT))
	{
		printf("%s '%s'\n", SYNTAXERR, "newline");
		return (1);
	}
	return (0);
}

int	check_quotes(char *line)
{
	int	i;
	int	d;
	int	s;

	i = 0;
	d = 0;
	s = 0;
	while (line[i])
	{
		if (d && line[i] == '"' && !s)
			d = 0;
		else if (!d && line[i] == '"' && !s)
			d = 1;
		if (s && line[i] == '\'' && !d)
			s = 0;
		else if (!s && line[i] == '\'' && !d)
			s = 1;
		i++;
	}
	if (d != 0 || s != 0)
		return (1);
	return (0);
}

int	check_syntax(t_chain *list, char *line, int l_paren, int r_paren)
{
	if (list && !list->next && one_token(list))
		return (1);
	else if (!(list && !list->next))
	{
		while (list)
		{
			if (list->type == L_PAREN)
				l_paren++;
			else if (list->type == R_PAREN)
				r_paren++;
			if (multiple_tokens(list->back, list, list->next))
			{
				list->error = 1;
				return (1);
			}
			list = list->next;
		}
	}
	if (l_paren != r_paren || check_quotes(line))
	{
		if (l_paren != r_paren)
			printf("minishell: syntax error: unclosed parenthesis\n");
		else
			printf("minishell: syntax error: unquoted string\n");
		return (1);
	}
	return (0);
}