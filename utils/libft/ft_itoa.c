/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 13:42:29 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 21:12:42 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_len(int n)
{
	size_t	len;

	len = 0;
	if (n < 0)
		len++;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	size_t	i;
	char	*ptr;
	long	num;

	num = n;
	i = ft_len(n);
	ptr = malloc(sizeof(char) * (i + 1));
	if (!ptr)
		return (NULL);
	ptr[i--] = '\0';
	if (n < 0)
	{
		ptr[0] = '-';
		num = num * -1;
	}
	while (num > 0)
	{
		ptr[i] = (num % 10) + '0';
		num /= 10;
		i--;
	}
	if (n == 0)
		ptr[0] = '0';
	return (ptr);
}
