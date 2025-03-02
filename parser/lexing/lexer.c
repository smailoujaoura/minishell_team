/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:51 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/02 19:55:18 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// looks into the next character if it is in a set of tokens
int	peek(char *s, char *tokens)
{
	char	*tok;

	tok = tokens;
	while (*tok)
	{
		if (*s == *tok)
			return (1);
		tok++;
	}
	if (*s == '&' && s + 1 && *(s + 1) == '&')
		return (1);
	return (0);
}

// tokenize the list depending on the first one/two characters > << ( || ..
void	tokenize_list(t_chain *list)
{
	while (list)
	{
		if (list->content[0] == '(')
			list->type = L_PAREN;
		else if (list->content[0] == ')')
			list->type = R_PAREN;
		else if (list->content[0] == '|' && list->content[1] == '|')
			list->type = OR;
		else if (list->content[0] == '|')
			list->type = PIPE;
		else if (list->content[0] == '&' && list->content[1] == '&')
			list->type = AND;
		else if (list->content[0] == '>' && list->content[1] == '>')
			list->type = REDIR_APPEND;
		else if (list->content[0] == '>')
			list->type = REDIR_OUT;
		else if (list->content[0] == '<' && list->content[1] == '<')
			list->type = HEREDOC;
		else if (list->content[0] == '<')
			list->type = REDIR_IN;
		else
			list->type = WORD;
		list = list->next;
	}
}

// keeps incrementing the start until a space or real operator is found
void	handle_words(char **start)
{
	if (**start == '\0' || ft_strchr(WHITESPACE, **start)
		|| peek(*start, "|<>()") || (**start == '&' && *(*start + 1) == '&'))
		return ;
	else if (**start == '"')
		handle_quotes(start, '"');
	else if (**start == '\'')
		handle_quotes(start, '\'');
	else
		while (!ft_strchr(WHITESPACE, **start) && !ft_strchr(SYMBOLS, **start)
			&& !(**start == '&' && *(*start + 1) == '&'))
			(*start)++;
	handle_words(start);
}

// loops through the string searching for complete token and returns it
int	get_token(char **start, char **tok_start, char **tok_end)
{
	while (**start && ft_strchr(WHITESPACE, **start))
		(*start)++;
	*tok_start = *start;
	if (**start == '\0')
		return (0);
	else if (**start == '(' || **start == ')')
		(*start)++;
	else if (**start == '<' || **start == '>')
		handle_redirs(start);
	else if (**start == '|')
	{
		(*start)++;
		if (**start == '|')
			(*start)++;
	}
	else if (**start == '&' && *(*start + 1) == '&')
		(*start) += 2;
	else
		handle_words(start);
	*tok_end = *start;
	return (1);
}

// keeps adding tokens into the list of tokens t_chain
void	convert_str(char *str, t_chain **list)
{
	char	*tok_start;
	char	*tok_end;

	while (get_token(&str, &tok_start, &tok_end))
		lstadd_back(list,
			lstnew(ft_substr(tok_start, 0, tok_end - tok_start, SOUJAOUR)));
}
