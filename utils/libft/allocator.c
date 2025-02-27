#include "libft.h"
#include "../../includes/minishell.h"

void	*check_collectors(size_t size, int flag)
{
    char    *ptr;

    ptr = NULL;
    if (flag == BKOLANI)
        ptr = ft_malloc_bkol(size, ALLOCATE);
    else
        ptr = ft_malloc(size, ALLOCATE);
    return (ptr);
}