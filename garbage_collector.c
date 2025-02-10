/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:39:12 by soujaour          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/02/10 15:40:13 by bkolani          ###   ########.fr       */
=======
/*   Updated: 2025/02/10 14:27:49 by soujaour         ###   ########.fr       */
>>>>>>> 335877d9de2a876a237ad405437f23878e4d80cb
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	panic_exit(char *s)
{
	printf("%s\n", s);
	exit(1);
}

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
		garbage_collector(allocs, NULL, NULL);
	else
	{
		ptr = malloc(size);
		new = malloc(sizeof(t_list));
		if (ptr == NULL || new == NULL)
		{
			garbage_collector(allocs, ptr, new);
			ft_malloc_bkol(0, DEALLOCATE);
			panic_exit(MEMORY_ERROR);
		}
		new->content = ptr;
		new->next = NULL;
		ft_lstadd_front(&allocs, new);
	}
	return (ptr);
}