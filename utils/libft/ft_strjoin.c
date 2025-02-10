/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:36:16 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/10 11:29:41 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	ptr = malloc(sizeof(char) * (ft_strlen((char *)s1)
				+ ft_strlen((char *)s2) + 1));
	ptr = ft_malloc(ft_strlen(s1) + ft_strlen(s2) + 1, ALLOCATE);
	if (!ptr)
		return (NULL);
	j = 0;
	while (*s1)
	{
		ptr[j++] = *s1++;
	}
	while (*s2)
	{
		ptr[j++] = *s2++;
	}
	ptr[j] = '\0';
	return (ptr);
}
