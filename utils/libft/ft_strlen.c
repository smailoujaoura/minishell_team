/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:20:52 by soujaour          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/02/05 10:11:04 by soujaour         ###   ########.fr       */
=======
/*   Updated: 2025/02/04 18:28:59 by bkolani          ###   ########.fr       */
>>>>>>> c42d50caca06aee4b13ec718a1dcfe164a4aa2d9
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}
