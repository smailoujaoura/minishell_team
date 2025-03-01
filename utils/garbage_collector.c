/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:39:12 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/01 10:49:41 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	garbage_collector(t_list *allocs, void *one, void *two)
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
}

void	*ft_malloc(size_t size, int flag)
{
	void			*ptr;
	t_list			*new;
	static t_list	*allocs;

	ptr = NULL;
	if (flag == DEALLOCATE)
	{
		garbage_collector(allocs, NULL, NULL);
		allocs = NULL;
	}
	else
	{
		ptr = malloc(size);
		new = malloc(sizeof(t_list));
		if (ptr == NULL || new == NULL)
		{
			garbage_collector(allocs, ptr, new);
			ft_malloc_bkol(0, DEALLOCATE);
			panic_exit(MEMORY_ERROR, 100);
		}
		new->content = ptr;
		new->next = NULL;
		ft_lstadd_front(&allocs, new);
	}
	return (ptr);
}
