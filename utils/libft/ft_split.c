/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:39:18 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 21:12:35 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_word_len(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static int	ft_words_counter(const char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || !s[i + 1]))
			count++;
		i++;
	}
	return (count);
}

char	**ft_split(char const *s, char c, int f)
{
	char	**arr;
	int		i;
	char	*alt_s;

	if (!s)
		return (NULL);
	alt_s = (char *)s;
	arr = (char **)check_collectors((sizeof(char *)
				* (ft_words_counter(s, c) + 1)), f);
	i = 0;
	while (i < ft_words_counter(s, c))
	{
		while (*alt_s == c)
			alt_s++;
		arr[i] = check_collectors((ft_word_len(alt_s, c) + 1), f);
		ft_strlcpy(arr[i], alt_s, ft_word_len(alt_s, c) + 1);
		alt_s += ft_word_len(alt_s, c);
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
