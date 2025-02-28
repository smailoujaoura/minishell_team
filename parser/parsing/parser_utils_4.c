/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:39 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/28 18:17:30 by soujaour         ###   ########.fr       */
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