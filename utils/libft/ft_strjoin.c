/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:36:16 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/10 21:12:57 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2, int f)
{
	char	*ptr;
	size_t	j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2, f));
	if (!s2)
		return (ft_strdup(s1, f));
	ptr = check_collectors((ft_strlen(s1) + ft_strlen(s2) + 1), f);
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
