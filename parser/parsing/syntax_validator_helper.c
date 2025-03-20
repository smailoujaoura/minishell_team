/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:41:27 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/20 17:58:10 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_pipe(t_chain *prev, t_chain *current, t_chain *next)
{
	if (next == NULL)
		return (0);
	if (prev == NULL || (prev->type != WORD && prev->type != R_PAREN))
	{
		write_four_strings(SYNTAX, " `", current->content, "'\n");
		return (1);
	}
	if (next->type == PIPE || next->type == AND || next->type == OR
		|| next->type == R_PAREN)
	{
		if (!next)
		{
			write(2, SYNTAX, ft_strlen(SYNTAX));
			write(2, " 'newline'\n", 11);
		}
		else if (!prev)
		{
			write(2, SYNTAX, ft_strlen(SYNTAX));
			write(2, " `|'\n", 5);
		}
		else
			write_four_strings(SYNTAX, " `", next->content, "'\n");
		return (1);
	}
	return (0);
}

int	check_logicals(t_chain *prev, t_chain *current, t_chain *next)
{
	if (next == NULL)
		return (0);
	if (prev == NULL || (prev->type != WORD && prev->type != R_PAREN))
	{
		write_four_strings(SYNTAX, " `", current->content, "'\n");
		return (1);
	}
	if (next->type == PIPE || next->type == AND || next->type == OR
		|| next->type == R_PAREN)
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

int	check_l_paren(t_chain *token, int left __attribute__((unused)), int right __attribute__((unused)))
{
	t_chain	*prev;
	t_chain	*next;

	if (token)
	{
		prev = token->back;
		next = token->next;
	}
	if (next == NULL)
	{
		write_four_strings(SYNTAX, "`('\n", NULL, NULL);
		return (1);
	}
	if (prev && (prev->type == R_PAREN || prev->type == WORD))
	{
		write_four_strings(SYNTAX, " `", next->content, "'\n");
		return (1);
	}
	if (next && (next->type == R_PAREN || next->type == AND
		|| next->type == OR || next->type == PIPE))
	{
		write_four_strings(SYNTAX, " `", next->content, "'\n");
		return (1);
	}
	return (0);
}

int	check_r_paren(t_chain *token, int left, int right)
{
	t_chain	*prev;
	t_chain	*next;

	if (token)
	{
		prev = token->back;
		next = token->next;
	}
	if (left != right)
	{
		write_four_strings(SYNTAX, "`)'\n", NULL, NULL);
		return (1);
	}
	if (next == NULL)
		return (0);
	if (prev == NULL)
	{
		write_four_strings(SYNTAX, "`)'\n", NULL, NULL);
		return (1);
	}
	if (next && (next->type == WORD || next->type == L_PAREN))
	{
		write_four_strings(SYNTAX, " `", next->content, "'\n");
		return (1);
	}
	return (0);
}
