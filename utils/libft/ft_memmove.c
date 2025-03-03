/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:33:44 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/03 12:21:38 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*dst_ptr;
	char	*src_ptr;

	if (!dst && !src)
		return (NULL);
	dst_ptr = (char *)dst;
	src_ptr = (char *)src;
	if (dst_ptr < src_ptr)
	{
		while (len--)
			*dst_ptr++ = *src_ptr++;
	}
	else
	{
		dst_ptr += len;
		src_ptr += len;
		while (len--)
		{
			*(--dst_ptr) = *(--src_ptr);
		}
	}
	return (dst);
}
