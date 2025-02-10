/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 09:06:34 by soujaour          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/02/10 15:38:11 by bkolani          ###   ########.fr       */
=======
/*   Updated: 2025/02/10 14:26:25 by soujaour         ###   ########.fr       */
>>>>>>> 335877d9de2a876a237ad405437f23878e4d80cb
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len, int f)
{
	size_t	i;
	size_t	s_len;
	char	*ptr;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup("", f));
	if (len > s_len - start)
		len = s_len - start;
	ptr = check_collectors((len + 1), f);
	i = 0;
	while (i < len)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
