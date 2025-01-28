/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:25:45 by soujaour          #+#    #+#             */
/*   Updated: 2024/12/30 16:24:04 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr_gnl(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strjoin_gnl(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup_gnl(s2));
	if (!s2)
		return (ft_strdup_gnl(s1));
	ptr = malloc((ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1));
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

char	*ft_strdup_gnl(const char *s1)
{
	char	*ptr;
	size_t	i;

	i = 0;
	while (s1[i])
		i++;
	ptr = malloc(sizeof(char) * (i + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_substr_gnl(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	s_len;
	char	*ptr;

	if (!s)
		return (NULL);
	s_len = ft_strlen_gnl((char *)s);
	if (start >= s_len)
	{
		return (ft_strdup_gnl(""));
	}
	if (len > s_len - start)
		len = s_len - start;
	ptr = malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

size_t	ft_strlcpy_gnl(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	s_len;

	i = 0;
	s_len = 0;
	while (src && src[s_len])
		s_len++;
	if (dstsize == 0)
		return (s_len);
	while (src && src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (s_len);
}
