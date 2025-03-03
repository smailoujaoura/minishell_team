/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:50:27 by soujaour          #+#    #+#             */
/*   Updated: 2024/10/24 15:59:12 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_set_check(const char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	char	*ptr;

	if (!s1 || !set)
		return (NULL);
	while (*s1)
	{
		if (ft_set_check(*s1, set))
			s1++;
		else
			break ;
	}
	len = ft_strlen((char *)s1);
	while (len > 0)
	{
		if (ft_set_check(*(s1 + len - 1), set))
			len--;
		else
			break ;
	}
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s1, len + 1);
	return (ptr);
}
