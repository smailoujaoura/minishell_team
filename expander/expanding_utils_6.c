/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils_6.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:27:06 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 20:36:55 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// convert the linked list of expansions into an array argv
char	**convert_list_str(t_list *list)
{
	int		i;
	int		len;
	char	**array;

	i = 0;
	len = ft_lstsize(list);
	array = ft_calloc(len + 1, sizeof(char *), SOUJAOUR);
	while (list)
	{
		array[i] = ft_strdup(list->content, SOUJAOUR);
		list = list->next;
		i++;
	}
	return (array);
}

// handle the special case where "$non" should be kept as part of the argv arr 
void	handle_special_case(t_list **list, char **str, char **flags)
{
	expand_wildcard(list, ft_strdup("", SOUJAOUR), "");
	(*flags)++;
	(*str)++;
}

// ultimately splitting the string that was expanded, joined, expanded, joined
// into an array of string that will be passed to execve
char	**ultimate_split(char *str, char *flags, char sep, t_list *list)
{
	char	*trail;
	char	*pattern;
	char	*is_wild;

	while (*flags)
	{
		while (*flags == sep)
		{
			flags++;
			str++;
		}
		if (*flags == HANDLE)
			handle_special_case(&list, &str, &flags);
		else if (*flags)
		{
			trail = flags;
			while (*flags && *flags != sep)
				flags++;
			pattern = ft_substr(str, 0, flags - trail, SOUJAOUR);
			is_wild = ft_substr(trail, 0, flags - trail, SOUJAOUR);
			expand_wildcard(&list, pattern, is_wild);
			str += flags - trail;
		}
	}
	return (convert_list_str(list));
}
