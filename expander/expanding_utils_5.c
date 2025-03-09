/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_5.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:40 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 20:44:56 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// function to check if str matches pattern 
// pattern string contains wildcards which are only valid if is_wild confirms
int	match_wildcard(char *str, char *pattern, char *is_wild)
{
	while (*pattern)
	{
		if (*pattern == '*' && *is_wild == IS_WILD)
		{
			while (*is_wild == IS_WILD && *pattern == '*')
				1 && (pattern++, is_wild++);
			if (!*pattern)
				return (1);
			while (*str)
			{
				if (match_wildcard(str, pattern, is_wild))
					return (1);
				str++;
			}
			return (0);
		}
		if (*pattern != *str)
			return (0);
		pattern++;
		str++;
		is_wild++;
	}
	return (*str == '\0');
}

// bubble sort a linked list by ascii 
void	bubble_ascii_sort(t_list *list)
{
	char	*tmp;
	t_list	*saved;
	int		swapped;

	swapped = 1;
	saved = list;
	while (swapped)
	{
		swapped = 0;
		while (list->next)
		{
			if (ft_strncmp(list->content, list->next->content, INT32_MAX) > 0)
			{
				tmp = list->content;
				list->content = list->next->content;
				list->next->content = tmp;
				swapped = 1;
			}
			list = list->next;
		}
		list = saved;
	}
}

// sort the added entries in a linked list of expansions and literals
void	sort_newly_added(int action, t_list *list)
{
	static int	starting_point;
	int			i;

	if (action == RECORD)
	{
		starting_point = 0;
		while (list)
		{
			starting_point++;
			list = list->next;
		}
	}
	else if (action == SORT)
	{
		i = 0;
		while (i != starting_point)
		{
			list = list->next;
			i++;
		}
		bubble_ascii_sort(list);
	}
}

// check all entries of a current direction if they match the pattern for
void	check_entries(t_list **list, char *pattern, char *is_wild, int *found)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			entry = readdir(dir);
		else
		{
			if (match_wildcard(entry->d_name, pattern, is_wild))
			{
				ft_lstadd_back(list,
					ft_lstnew(ft_strdup(entry->d_name, SOUJAOUR)));
				*found = 1;
			}
			entry = readdir(dir);
		}
	}
	closedir(dir);
}

// expands willcard and sorts the elements found in that expansion
void	expand_wildcard(t_list **list, char *pattern, char *is_wild)
{
	int				found;

	found = 0;
	if (ft_strchr(is_wild, IS_WILD))
	{
		sort_newly_added(RECORD, *list);
		check_entries(list, pattern, is_wild, &found);
		if (found)
			sort_newly_added(SORT, *list);
	}
	if (!ft_strchr(is_wild, IS_WILD) || found == 0)
		ft_lstadd_back(list, ft_lstnew(pattern));
}
