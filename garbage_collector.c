/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:39:12 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/09 21:47:10 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	garbage_collector(t_list *new, void *one, void *two, int flag)
{
	static t_list	*allocs;
	t_list			*tmp;

	if (flag == ALLOCATE)
	{
		ft_lstadd_front(&allocs, new);
		return ;
	}
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
	void	*ptr;
	t_list	*new;

	if (flag == DEALLOCATE)
		garbage_collector(NULL, NULL, NULL, DEALLOCATE);
	else
	{
		ptr = malloc(size);
		new = malloc(sizeof(t_list));
		if (ptr == NULL || new == NULL)
		{
			
		}
		new->content = ptr;
		new->next = NULL;
	}
	return (ptr);
}