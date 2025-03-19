/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:41:27 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/19 09:12:25 by soujaour         ###   ########.fr       */
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
		write_four_strings(SYNTAX, " `", current->content, "\n");
		return (1);
	}
	if (next->type == PIPE || next->type == AND || next->type == OR || !prev)
	{
		if (!next)
		{
			write(2, SYNTAX, ft_strlen(SYNTAX));
			write(2, " 'newline'\n", 12);
		}
		else if (!prev)
		{
			write(2, SYNTAX, ft_strlen(SYNTAX));
			write(2, " `|'\n", 4);
		}
		else
			write_four_strings(SYNTAX, " ```", next->content, "'\n");
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
		write_four_strings(SYNTAX, " `", current->content, "\n");
		return (1);
	}
	if (next->type == PIPE || next->type == AND || next->type == OR)
	{
		if (!next)
		{
			write_four_strings(SYNTAX, " `", "newline", "'\n");
		}
		else
		{
			write_four_strings(SYNTAX, " `", next->content, "'\n");
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
			write(2, SYNTAX, ft_strlen(SYNTAX));
			write(2, " `", 2);
			write(2, "newline", 7);
			write(2, "'\n", 2);
		}
		else
		{
			write(2, SYNTAX, ft_strlen(SYNTAX));
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
		if (prev && (prev->type != WORD || prev->type != OR
			|| prev->type != AND || prev->type != PIPE))
			return (write_four_strings(SYNTAX, "` ", next->content, "'\n"), 1);
		if (next->type == R_PAREN || next->type == AND
			|| next->type == OR || next->type == PIPE)
			return (write_four_strings(SYNTAX, " `", next->content, "'\n"), 1);
	}
	if (paren == R_PAREN)
	{
		if (next->type == WORD || next->type == L_PAREN)
			return (write_four_strings(SYNTAX, " `", next->content, "'\n"), 1);
	}
	return (0);
}
