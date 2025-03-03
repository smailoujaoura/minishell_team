/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 07:53:27 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/01 10:56:47 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// we need stack behaviour sometimes so we specify that with f(flag) = 1,
// otherwise f = 0, meaning adding the new node to the back of the list
// save node to be moved in tmp; and update src to point to subsequent one
// this is for protecting when the stack is empty in order not to segfault 
void	move_item(t_chain **src, t_chain **dst, int f)
{
	t_chain	*tmp;

	if (!dst || !src || !(*src))
		return ;
	if (f == 0)
	{
		tmp = *src;
		*src = tmp->next;
		if (*src)
			(*src)->back = NULL;
		tmp->next = NULL;
		lstadd_back(dst, tmp);
		return ;
	}
	tmp = *src;
	*src = tmp->next;
	if (*src)
		(*src)->back = NULL;
	tmp->back = NULL;
	tmp->next = *dst;
	*dst = tmp;
}

void	delete_one(t_chain **list, int i)
{
	t_chain	*tmp;

	if (!list || !(*list))
		return ;
	tmp = *list;
	*list = tmp->next;
	if (*list)
		(*list)->back = NULL;
	(void)i;
}

t_chain	*lstnew(char *content)
{
	t_chain	*new;

	new = ft_malloc(sizeof(t_chain), ALLOCATE);
	new->content = content;
	new->type = -1;
	new->lvl = -1;
	new->empty = 0;
	new->next = NULL;
	new->back = NULL;
	new->delim = NULL;
	new->file = NULL;
	new->adj_f = NULL;
	new->argv = NULL;
	new->removable = 0;
	new->delim_in_quotes = 0;
	new->ambiguous = 0;
	new->error = 0;
	return (new);
}

t_chain	*lstlast(t_chain *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

void	lstadd_back(t_chain **lst, t_chain *new)
{
	t_chain	*last;

	if (!lst || !new)
		return ;
	if (*lst)
	{
		last = lstlast(*lst);
		last->next = new;
		new->back = last;
	}
	else
	{
		new->back = NULL;
		*lst = new;
	}
}
