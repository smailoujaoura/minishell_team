/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:39 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/28 22:15:31 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_redir(t_chain *ptr, int f)
{
	if (!ptr)
		return (0);
	if (f == IN + OR + OUT && (is_redir(ptr, IN) || is_redir(ptr, OUT)))
		return (1);
	if (f == IN && (ptr->type == HEREDOC || ptr->type == REDIR_IN))
		return (1);
	else if (f == OUT && (ptr->type == REDIR_APPEND || ptr->type == REDIR_OUT))
		return (1);
	return (0);
}

void	delete_any(t_chain *ptr, int i)
{
	t_chain	*prev;

	prev = NULL;
	if (ptr->back != NULL)
		prev = ptr->back;
	prev->next = ptr->next;
	if (ptr->next)
		ptr->next->back = prev;
	(void)i;
}

void	remove_if(t_chain *list)
{
	t_chain	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->type == REMOVE || tmp->removable == REMOVE)
			delete_any(tmp, 0);
	}
}
