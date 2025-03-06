/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 20:42:25 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 20:45:48 by soujaour         ###   ########.fr       */
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

t_chain	*create_empty(t_chain *start)
{
	t_chain	*new;

	new = lstnew(ft_strdup("EMPTY CMD!", SOUJAOUR));
	new->type = WORD;
	new->empty = 1;
	new->adj_f = start;
	return (new);
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
		return (create_empty(start));
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
