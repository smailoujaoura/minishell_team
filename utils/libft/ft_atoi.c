/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:58:09 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/03 15:10:44 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atoi(const char *str, int *error)
{
	int		sign;
	long	num;
	long	tmp;

	num = 0;
	tmp = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str++ == '-')
			sign *= -1;
	}
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			*error = 1;
		num = num * 10 + (*str - '0');
		if ((num < tmp && sign == 1) || (num < tmp && sign == -1))
			*error = 1;
		tmp = num;
		str++;
	}
	return (num * sign);
}
