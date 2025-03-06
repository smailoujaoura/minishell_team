/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:58:09 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 21:38:17 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	atoi_help(const char *str, int *sign)
{
	if (*str == '-' || *str == '+')
	{
		if (*str++ == '-')
			*sign *= -1;
	}
	return (*sign);
}

long	ft_atoi(const char *str, int *error)
{
	int		sign;
	long	num;
	long	tmp;

	num = 0;
	tmp = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	sign = atoi_help(str, &sign);
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			*error = 1;
		num = num * 10 + (*str - '0');
		if (num < tmp && sign == 1)
			*error = 1;
		if (num < tmp && sign == -1)
			*error = 1;
		tmp = num;
		str++;
	}
	return (num * sign);
}
