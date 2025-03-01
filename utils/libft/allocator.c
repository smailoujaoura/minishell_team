/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 10:47:35 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/01 10:48:14 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../includes/minishell.h"

void	*check_collectors(size_t size, int flag)
{
	char	*ptr;

	ptr = NULL;
	if (flag == BKOLANI)
		ptr = ft_malloc_bkol(size, ALLOCATE);
	else
		ptr = ft_malloc(size, ALLOCATE);
	return (ptr);
}
