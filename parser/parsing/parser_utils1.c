/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:23 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/24 11:09:18 by soujaour         ###   ########.fr       */
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

char	*copy_if(char *str, char *s, char *f)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (s[i] == 0)
		{
			f[k] = str[i];
			k++;
		}
		i++;
	}
	f[k] = '\0';
	return (f);
}

int	count_removables(char *s)
{
	int i;
	int	rem;

	i = 0;
	rem = 0;
	while (s[i])
	{
		if (s[i] == 1)
			rem++;
		i++;
	}
	return (rem);
}

char	*remove_occurences(char *str, int i, int singles, int doubles)
{
	char	*s;
	char	*f;

	s = ft_calloc(ft_strlen(str) + 1, 1, SOUJAOUR);
	while (str[i])
	{
		if (str[i] == '"' && singles != 1)
			doubles++;
		if (str[i] == '\'' && doubles != 1)
			singles++;
		if (doubles == 2)
			doubles = 0;
		if (singles == 2)
			singles = 0;
		if (singles != 1 && doubles != 2 && (str[i] == '"' || str[i] == '\''))
			s[i] = 1;
		i++;
	}
	f = ft_malloc(ft_strlen(str) - count_removables(s) + 1, ALLOCATE);
	return (copy_if(str, s, f));
}