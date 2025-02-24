#include "minishell.h"

void	garbage_coll_bkol(t_list *allocs, void *one, void *two)
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

void	*ft_malloc_bkol(size_t size, int flag)
{
	void			*ptr;
	t_list			*new;
	static t_list	*allocs;

	ptr = NULL;
	if (flag == DEALLOCATE)
		garbage_coll_bkol(allocs, NULL, NULL);
	else
	{
		ptr = malloc(size);
		new = malloc(sizeof(t_list));
		if (ptr == NULL || new == NULL)
		{
			// garbage_collector(NULL, NULL, NULL);
			garbage_coll_bkol(allocs, ptr, new);
			printf("Panic exit\n");
		}
		new->content = ptr;
		new->next = NULL;
		ft_lstadd_front(&allocs, new);
	}
	return (ptr);
}