/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:39:12 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/09 17:46:32 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	garbage_collector(void *new, int flag)
{
	
}

void	*ft_malloc(size_t size, int flag)
{
	void	*ptr;

	if (flag == DEALLOCATE)
	{
		
	}
	else
	{
		ptr = malloc(size);
		
	}
}