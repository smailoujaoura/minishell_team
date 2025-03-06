/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:31 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 12:04:58 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	lefts_picker(t_chain *list)
{
	t_chain	*ptr;
	t_chain	*new;

	while (list)
	{
		ptr = list;
		while (ptr && is_redir(ptr, IN + OR + OUT))
			ptr = ptr->next;
		if (ptr != list)
		{
			new = ft_calloc(1, sizeof(t_chain), SOUJAOUR);
			new->type = WORD;
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

void	post_picker(t_chain *list)
{
	t_chain	*start;
	t_chain	*end;
	t_chain	*last_redir;

	if (!list->next || !is_redir(list->next, IN + OR + OUT))
		return ;
	start = list->next;
	end = list->next;
	while (end->next && is_redir(end->next, IN + OR + OUT))
	{
		end = end->next;
	}
	list->next = end->next;
	end->next = NULL;
	start->back = NULL;
	if (list->adj_f)
	{
		last_redir = lstlast(list->adj_f);
		last_redir->next = start;
		start->back = last_redir;
	}
	else
		list->adj_f = start;
}

void	pre_picker(t_chain *list)
{
	t_chain	*start;
	t_chain	*end;

	if (!list->back || !is_redir(list->back, IN + OR + OUT))
		return ;
	start = list->back;
	end = list->back;
	while (list->back && is_redir(list->back, IN + OR + OUT))
	{
		list = list->back;
	}
	start->back = NULL;
	end->next = NULL;
	list->adj_f = start;
}

t_chain	*special_redir_case(t_chain *list)
{
	t_chain	*start;
	t_chain	*end;
	t_chain	*new;

	start = list;
	end = list;
	while (end && is_redir(end, IN + OR + OUT))
		end = end->next;
	if (end == NULL)
	{
		new = lstnew(ft_strdup("EMPTY CMD!", SOUJAOUR));
		new->type = WORD;
		new->empty = 1;
		new->adj_f = start;
		return (new);
	}
	else if (end->type != WORD)
	{
		new = lstnew(ft_strdup("EMPTY CMD", SOUJAOUR));
		new->adj_f = start;
		end->back->next = NULL;
		end->back = new;
		new->next = end;
		return (new);
	}
	end->adj_f = start;
	end->back->next = NULL;
	end->back = NULL;
	return (end);
}

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
			while (ptr && (ptr->type == WORD
					|| ptr->type == WILDCARD || is_redir(ptr, IN + OR + OUT)))
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
