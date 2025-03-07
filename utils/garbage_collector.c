/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:39:12 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/07 09:00:44 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	garbage_collector(t_list *allocs, void *one, void *two, int f)
{
	t_list			*tmp;

	if (one)
		free(one);
	if (two)
		free(two);
	while (allocs)
	{
		tmp = allocs->next;
		free(allocs->content);
		free(allocs);
		allocs = tmp;
	}
	if (f)
	{
		perror("minishell");
		exit(EXIT_FAILURE);
	}
}

void	*ft_malloc(size_t size, int flag)
{
	void			*ptr;
	t_list			*new;
	static t_list	*allocs;

	ptr = NULL;
	if (flag == DEALLOCATE)
	{
		garbage_collector(allocs, NULL, NULL, 0);
		allocs = NULL;
	}
	else
	{
		ptr = malloc(size);
		new = malloc(sizeof(t_list));
		if (ptr == NULL || new == NULL)
		{
			garbage_collector(allocs, ptr, new, 1);
			ft_malloc_bkol(0, DEALLOCATE);
		}
		new->content = ptr;
		new->next = NULL;
		ft_lstadd_front(&allocs, new);
	}
	return (ptr);
}
