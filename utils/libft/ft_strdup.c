/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:26:22 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/19 21:15:39 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1, int f)
{
	char	*ptr;
	size_t	i;

	i = 0;
	if (s1 == NULL)
		return (NULL);
	while (s1[i])
		i++;
	ptr = check_collectors((i + 1), f);
	i = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
