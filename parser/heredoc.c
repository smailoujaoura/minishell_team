/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:34:47 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/14 13:46:17 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_heredocs(t_chain *ptr)
{
	while (ptr)
	{
		if (ptr->type == HEREDOC)
		{
			close(ptr->fd);
		}
		ptr = ptr->back;
	}
}

// remove here-document's delimiter's quotes and open it, store its contents
int	open_heredocs(t_chain *list, t_shell *mini)
{
	int	status;

	status = 0;
	count_heredocs(list);
	while (list)
	{
		if (list->error == -1)
			return (1);
		if (list->type == HEREDOC)
		{
			strip_heredoc(list, list->next->content);
			delete_any(list->next, 0);
			status = here_doc(list, mini);
			if (status == EXIT_FAILURE)
			{
				close_heredocs(list);
				return (1);
			}
		}
		list = list->next;
	}
	return (0);
}
