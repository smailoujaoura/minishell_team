/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:31 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/24 11:09:04 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	pick_left_redirs(t_chain *list)
{
	t_chain	*ptr;
	t_chain	*new;

	while (list)
	{
		ptr = list;
		while (ptr && (is_redir(ptr, IN + OR + OUT)))
			ptr = ptr->next;
		if (ptr != list)
		{
			new = ft_calloc(1, sizeof(t_chain), SOUJAOUR);
			new->type = WORD;
			// GIVE CONTENT "empty for now"
			new->content = "Empty CMD";
			new->empty = 1;
			new->next = ptr;
			new->back = list->back;
			list->back = NULL;
			new->adj_f = list;
			new->back->next = new;
			list = new;
		}
		list = list->next;
	}
}

t_chain	*assign_inputs(t_chain *list, t_chain *ptr)
{
	list = assign_inputs_edges(list);
	assign_adjacent_redirs(list, ptr);
	remove_if(list);
	return (list);
}
void	join_commands(t_chain *list)
{
	t_argv	*argv;
	t_argv	*new;
	t_chain	*ptr;

	while (list)
	{
		argv = NULL;
		if (list->type == WORD)
		{
			ptr = list->next;
			while (ptr && (ptr->type == WORD || ptr->type == WILDCARD || is_redir(ptr, IN + OR + OUT) ))
			{
				if (ptr->type == WORD || ptr->type == WILDCARD)
				{
					new = lstnew_arg(ptr);
					new->type = ptr->type;
					lstadd_back_arg(&argv, new);
					ptr->type = REMOVE;
				}
				ptr = ptr->next;
			}
			remove_if(list);
			list->argv = argv;
		}
		list = list->next;
	}
}


void	join_redirs(t_chain *list)
{
	while (list)
	{
		if (list->type == REDIR_IN || list->type == REDIR_OUT || list->type == REDIR_APPEND)
		{
			list->file = list->next->content;
			delete_any(list->next, 0);
		}
		if (list->type == HEREDOC)
		{
			list->delim = list->next->content;
			list->delim_in_quotes = list->next->delim_in_quotes;
			delete_any(list->next, 0);
		}
		list = list->next;
	}
}

void	strip_words(t_chain *list)
{
	while (list)
	{
		if (list->type == HEREDOC && list->next && list->next->type == WORD)
		{
			if (ft_strchr(list->next->content, '"') || ft_strchr(list->next->content, '\''))
				list->delim_in_quotes = 1;
		}
		if (list->type == WORD)
			list->content = remove_occurences(list->content, 0, 0, 0);
		list = list->next;
	}
}

void	prioritize_list(t_chain *list)
{
	while (list)
	{
		if (list->type == L_PAREN || list->type == R_PAREN)
			list->lvl = NAN;
		else if (list->type == WORD || list->type == REDIR_APPEND || list->type
			== REDIR_IN || list->type == REDIR_OUT || list->type == HEREDOC) // HEREDOC REDIRS
			list->lvl = VIP;
		else if (list->type == PIPE)
			list->lvl = LVL1;
		else if (list->type == OR || list->type == AND)
			list->lvl = LVL2;
		list = list->next;
	}
}
