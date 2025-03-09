/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:31 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 20:39:49 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_chain	*assign_inputs(t_chain *list)
{
	t_chain	*saved;

	saved = list;
	if (list && is_redir(list, IN + OR + OUT))
	{
		list = special_redir_case(list);
		saved = list;
	}
	while (list)
	{
		if (list->type == WORD)
		{
			pre_picker(list);
			post_picker(list);
		}
		else if (list->type == R_PAREN)
		{
			post_picker(list);
		}
		list = list->next;
	}
	lefts_picker(saved);
	return (saved);
}

// join commands with their arguments: ls -la
void	join_commands(t_chain *list, t_argv *argv, t_argv *new)
{
	t_chain	*ptr;

	while (list)
	{
		argv = NULL;
		if (list->type == WORD)
		{
			ptr = list->next;
			while (ptr && (ptr->type == WORD || is_redir(ptr, IN + OR + OUT)))
			{
				if (ptr->type == WORD)
				{
					new = lstnew_arg(ptr);
					new->type = ptr->type;
					lstadd_back_arg(&argv, new);
					ptr->type = REMOVE;
				}
				ptr = ptr->next;
			}
			remove_if(list);
			lstadd_back_arg(&list->argv, argv);
		}
		list = list->next;
	}
}

// join redirs nodes: (>, <, >>) with their filename's nodes 
void	join_redirs(t_chain *list)
{
	while (list)
	{
		if (list->type == REDIR_IN
			|| list->type == REDIR_OUT || list->type == REDIR_APPEND)
		{
			list->file = list->next->content;
			delete_any(list->next, 0);
		}
		list = list->next;
	}
}

// add priority levels for each token type based Shunting Yard Algo
void	prioritize_list(t_chain *list)
{
	while (list)
	{
		if (list->type == L_PAREN || list->type == R_PAREN)
			list->lvl = NAN;
		else if (list->type == WORD || list->type == REDIR_APPEND || list->type
			== REDIR_IN || list->type == REDIR_OUT || list->type == HEREDOC)
			list->lvl = VIP;
		else if (list->type == PIPE)
			list->lvl = LVL1;
		else if (list->type == OR || list->type == AND)
			list->lvl = LVL2;
		list = list->next;
	}
}
