/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:39:18 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/17 10:42:04 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./utils/libft/libft.h"

# define SEPERATORS " \t"

int	is_seperator(char c, char *seps)
{
	while (*seps)
	{
		if (*seps == c)
			return (1);
		seps++;
	}
	return (0);
}

int	count_if(char *s, char *seps, int i, void *ptr_fun)
{
	int	one;
	int	two;
	int	counter;
	void (*func)(int *, int *, char);

	one = 0;
	two = 0;
	func = (void (*)(int *, int *, char))ptr_fun;
	counter = 0;
	while (s[i])
	{
		func(&one, &two, s[i]);
		if (!one && !two && !is_seperator(s[i], seps) && (is_seperator(s[i + 1], seps) || !s[i + 1]))
			counter++;
		i++;
	}
	return (counter);
}

int	skip_seperator(char *s, char *seps, void *ptr_fun, int flag)
{
	int	i;
	int	one;
	int	two;

	i = 0;
	one = 0;
	two = 0;
	void (*func)(int *, int *, char);
	func = (void (*)(int *, int *, char))ptr_fun;
	while (flag && s[i])
	{
		func(&one, &two, s[i]);
		if (one || two || !is_seperator(s[i], seps))
			return (i);
		i++;
	}
	while (s[i])
	{
		func(&one, &two, s[i]);
		if (!one && !two && (is_seperator(s[i], seps) || !s[i]))
			return (i);
		i++;
	}
	return (i);
}

char	**split_if(char *s, char *seps, void *ptr_fun)
{
	int		i;
	int		len;
	char	**arr;
	char	*trail;

	if (s == NULL || seps == NULL)
		return (NULL);
	i = 0;
	len = count_if(s, seps, 0, ptr_fun);
	arr = ft_malloc(sizeof(char *) * (len + 1), ALLOCATE);
	arr[len] = NULL;
	while (i < len)
	{
		s += skip_seperator(s, seps, ptr_fun, 1);
		trail = s;
		s += skip_seperator(s, seps, ptr_fun, 0);
		arr[i] = ft_substr(trail, 0, s - trail, SOUJAOUR);
		i++;
	}
	return (arr);
}