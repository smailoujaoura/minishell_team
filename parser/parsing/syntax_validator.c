/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:41:27 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/20 17:46:18 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_four_strings(char *s1, char *s2, char *s3, char *s4)
{
	if (s1)
		write(2, s1, ft_strlen(s1));
	if (s2)
		write(2, s2, ft_strlen(s2));
	if (s3)
		write(2, s3, ft_strlen(s3));
	if (s4)
		write(2, s4, ft_strlen(s4));
}

// calls the function to do the syntax checking based on token's type
int	multiple_tokens(t_chain *token, int left, int right)
{
	t_chain	*prev;
	t_chain	*next;

	if (token)
	{
		prev = token->back;
		next = token->next;
	}
	if (token->type == PIPE)
		return (check_pipe(prev, token, next));
	if (token->type == AND || token->type == OR)
		return (check_logicals(prev, token, next));
	if (token->type == HEREDOC || is_redir(token, OUT + OR + IN))
		return (check_redirs(prev, next));
	if (token->type == L_PAREN)
		return (check_l_paren(token, left, right));
	if (token->type == R_PAREN)
		return (check_r_paren(token, left, right));
	return (0);
}

// checks the single token is of type Word
int	one_token(t_chain *list)
{
	if (!(list->type == WORD))
	{
		write(2, SYNTAX, ft_strlen(SYNTAX));
		write(2, " `", 2);
		write(2, list->content, ft_strlen(list->content));
		write(2, "'\n", 2);
		return (1);
	}
	return (0);
}

// checks if all quotes have their match
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

// checks syntax based on multiple conditions one token or two
// also checks for unquoted strings and unclosed parenthesis
int	check_syntax(t_chain *list, char *line, int l_paren, int r_paren)
{
	if (list && !list->next && one_token(list))
		return (list->error = -1);
	else
	{
		while (list)
		{
			if (list->type == L_PAREN)
				l_paren++;
			else if (list->type == R_PAREN)
				r_paren++;
			if (multiple_tokens(list, l_paren, r_paren))
				return (list->error = -1);
			list = list->next;
		}
	}
	if (l_paren != r_paren || check_quotes(line))
	{
		if (l_paren != r_paren)
			write(2, "minishell: syntax error: unclosed parenthesis\n", 47);
		else
			write(2, "minishell: syntax error: unquoted string\n", 41);
		return (-1);
	}
	return (0);
}
