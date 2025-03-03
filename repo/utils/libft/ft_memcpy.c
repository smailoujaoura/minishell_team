/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:11:13 by soujaour          #+#    #+#             */
/*   Updated: 2024/10/28 19:04:43 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*dst_ptr;
	char	*src_ptr;

	dst_ptr = (char *)dst;
	src_ptr = (char *)src;
	if (!dst && !src)
		return (dst);
	while (n--)
	{
		*dst_ptr = *src_ptr;
		src_ptr++;
		dst_ptr++;
	}
	return (dst);
}
