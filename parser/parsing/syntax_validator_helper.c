/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:41:27 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 10:09:12 by soujaour         ###   ########.fr       */
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

int	check_pipe(t_chain *prev, t_chain *current, t_chain *next)
{
	if (next == NULL)
		return (0);
	if (prev == NULL || prev->type != WORD)
	{
		write_four_strings(SYNTAXERR, " `", current->content, "\n");
		return (1);
	}
	if (next->type == PIPE || next->type == AND || next->type == OR || !prev)
	{
		if (!next)
		{
			write(2, SYNTAXERR, ft_strlen(SYNTAXERR));
			write(2, " 'newline'\n", 12);
		}
		else if (!prev)
		{
			write(2, SYNTAXERR, ft_strlen(SYNTAXERR));
			write(2, " `|'\n", 4);
		}
		else
			write_four_strings(SYNTAXERR, " ```", next->content, "'\n");
		return (1);
	}
	return (0);
}

int	check_logicals(t_chain *prev, t_chain *current, t_chain *next)
{
	if (next == NULL)
		return (0);
	if (prev == NULL || prev->type != WORD)
	{
		write_four_strings(SYNTAXERR, " `", current->content, "\n");
		return (1);
	}
	if (next->type == PIPE || next->type == AND || next->type == OR)
	{
		if (!next)
		{
			write_four_strings(SYNTAXERR, " `", "newline", "'\n");
		}
		else
		{
			write_four_strings(SYNTAXERR, " `", next->content, "'\n");
		}
		return (1);
	}
	return (0);
}

int	check_redirs(t_chain *prev, t_chain *next)
{
	if (!next || next->type != WORD)
	{
		if (!next)
		{
			write(2, SYNTAXERR, ft_strlen(SYNTAXERR));
			write(2, " `", 2);
			write(2, "newline", 7);
			write(2, "'\n", 2);
		}
		else
		{
			write(2, SYNTAXERR, ft_strlen(SYNTAXERR));
			write(2, " `", 2);
			write(2, next->content, ft_strlen(next->content));
			write(2, "'\n", 2);
		}
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
			write(2, SYNTAXERR, ft_strlen(SYNTAXERR));
			write(2, " `", 2);
			write(2, next->content, ft_strlen(next->content));
			write(2, "'\n", 2);
			return (1);
		}
	}
	if (paren == R_PAREN && next->type == L_PAREN)
	{
		write(2, SYNTAXERR, ft_strlen(SYNTAXERR));
		write(2, " `", 2);
		write(2, next->content, ft_strlen(next->content));
		write(2, "'\n", 2);
		return (1);
	}
	(void)prev;
	return (0);
}
