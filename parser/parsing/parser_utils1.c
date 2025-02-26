/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:23 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/26 08:00:03 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

void	remove_adjacent_redirs(t_chain *list, t_chain *redirs, int f)
{
	t_chain	*non_redir;
	t_chain	*tmp;

	non_redir = redirs;
	while (non_redir && is_redir(non_redir, IN + OR + OUT))
	{
		if (f)
			non_redir = non_redir->back;
		else
			non_redir = non_redir->next;
	}
	if (f)
	{
		if (non_redir)
			non_redir->next = list;
		list->back = non_redir;
	}
	else
	{
		list->next = non_redir;
		if (non_redir)
			non_redir->back = list; // if non_redir is valid 
	}
	while (is_redir(redirs, IN + OR + OUT))
	{
		tmp = redirs->next;
		// free(redirs); // need to free content as well.
		redirs = tmp;
	}
}

t_chain	*create_redirs_chain(t_chain *list)
{
	t_chain	*redirs;
	t_chain	*new;

	redirs = NULL;
	while (list && is_redir(list, IN + OR + OUT))
	{
		list->removable = REMOVE;
		new = ft_calloc(1, sizeof(t_chain), SOUJAOUR);
		ft_memcpy(new, list, sizeof(t_chain));
		new->next = NULL;
		new->back = NULL;
		lstadd_back(&redirs, new);
		list = list->next;
	}
	return (redirs);
}

char	*copy_if(char *new, char *str, char *flag)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (flag[i] == 1)
		{
			new[k] = str[i];
			k++;
		}
		i++;
	}
	new[k] = '\0';
	return (new);
}

char	*remove_occurences(char *str, int i, int one, int two)
{
	char	*flag;
	char	*new;
	int		removables;

	removables = 0;
	flag = ft_calloc(ft_strlen(str) + 1, sizeof(char), SOUJAOUR);
	while (str[i])
	{
		if (str[i] == '"' && one != 1)
			two++;
		else if (str[i] == '\'' && two != 1)
			one++;
		else
			flag[i] = 1;
		if (two == 2 || one == 2)
			removables += 2;
		if (two == 2)
			two = 0;
		if (one == 2)
			one = 0;
		i++;
	}
	new = ft_malloc(ft_strlen(str) - removables + 1, ALLOCATE);
	copy_if(new, str, flag);
	return (new);
}