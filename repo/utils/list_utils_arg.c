/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 07:53:27 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/01 10:57:03 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_argv	*lstnew_arg(t_chain *cmd)
{
	t_argv	*new;

	new = ft_malloc(sizeof(t_argv), ALLOCATE);
	new->back = NULL;
	new->next = NULL;
	new->type = -1;
	new->content = ft_strdup(cmd->content, SOUJAOUR);
	return (new);
}

t_argv	*lstlast_arg(t_argv *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

void	lstadd_back_arg(t_argv **lst, t_argv *new)
{
	t_argv	*last;

	if (!lst || !new)
		return ;
	if (*lst)
	{
		last = lstlast_arg(*lst);
		last->next = new;
		new->back = last;
	}
	else
	{
		new->back = NULL;
		*lst = new;
	}
}
