/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bkol_garb_coll.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 18:20:14 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/07 09:00:39 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	garbage_coll_bkol(t_list *allocs, void *one, void *two, int f)
{
	t_list	*tmp;

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

void	*ft_malloc_bkol(size_t size, int flag)
{
	void			*ptr;
	t_list			*new;
	static t_list	*allocs;

	ptr = NULL;
	if (flag == DEALLOCATE)
	{
		garbage_coll_bkol(allocs, NULL, NULL, 0);
		allocs = NULL;
	}
	else
	{
		ptr = malloc(size);
		new = malloc(sizeof(t_list));
		if (ptr == NULL || new == NULL)
		{
			garbage_coll_bkol(allocs, ptr, new, 1);
			ft_malloc(0, DEALLOCATE);
		}
		new->content = ptr;
		new->next = NULL;
		ft_lstadd_front(&allocs, new);
	}
	return (ptr);
}
