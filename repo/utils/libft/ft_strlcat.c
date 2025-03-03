/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:57:33 by soujaour          #+#    #+#             */
/*   Updated: 2024/10/29 08:22:24 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	s_len;
	size_t	d_len;
	size_t	i;

	s_len = 0;
	while (src[s_len])
		s_len++;
	d_len = 0;
	while (dst[d_len])
		d_len++;
	if (dstsize <= d_len)
		return (dstsize + s_len);
	i = 0;
	while (src[i] && i < dstsize - d_len - 1)
	{
		dst[i + d_len] = src[i];
		i++;
	}
	dst[i + d_len] = '\0';
	return (d_len + s_len);
}
