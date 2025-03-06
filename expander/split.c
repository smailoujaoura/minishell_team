/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:39:18 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 20:18:33 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_isinset(char c, char *set)
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

static int	ft_word_len(char *s, char *set)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (ft_isinset(s[i], set))
			break ;
		i++;
	}
	return (i);
}

static int	ft_words_counter(const char *s, char *set)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (!ft_isinset(s[i], set) && (ft_isinset(s[i + 1], set) || !s[i + 1]))
			count++;
		i++;
	}
	return (count);
}

char	**ft_split_custom(char const *s, char *set, int f)
{
	char	**arr;
	int		i;
	char	*alt_s;

	if (!s)
		return (NULL);
	alt_s = (char *)s;
	arr = (char **)check_collectors((sizeof(char *)
				* (ft_words_counter(s, set) + 1)), f);
	i = 0;
	while (i < ft_words_counter(s, set))
	{
		while (ft_isinset(*alt_s, set))
			alt_s++;
		arr[i] = check_collectors((ft_word_len(alt_s, set) + 1), f);
		ft_strlcpy(arr[i], alt_s, ft_word_len(alt_s, set) + 1);
		alt_s += ft_word_len(alt_s, set);
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
