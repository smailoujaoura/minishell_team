/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:26:34 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/14 13:30:59 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sort_array(t_env **sorted, int size, int swap)
{
	int		i;
	t_env	*temp;

	swap = 1;
	while (swap)
	{
		swap = 0;
		i = 0;
		while (i < size - 1)
		{
			if (ft_strncmp(sorted[i]->key, sorted[i + 1]->key, SIZE_MAX) > 0)
			{
				swap = 1;
				temp = sorted[i];
				sorted[i] = sorted[i + 1];
				sorted[i + 1] = temp;
			}
			i++;
		}
	}
}

void	print_array(t_env **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (array[i]->value)
			printf("declare -x %s=\"%s\"\n", array[i]->key, array[i]->value);
		else
		{
			printf("declare -x %s\n", array[i]->key);
		}
		i++;
	}
}

// Handle export command without args
void	print_keys_values(t_env *env)
{
	int		i;
	int		size;
	t_env	*current;
	t_env	**array;

	size = ft_lstsize_env(env);
	if (size == 0)
		return ;
	array = ft_malloc(sizeof(t_env *) * (size + 1), ALLOCATE);
	array[size] = NULL;
	i = 0;
	current = env;
	while (i < size)
	{
		array[i] = current;
		current = current->next;
		i++;
	}
	sort_array(array, size, 1);
	print_array(array);
}

int	is_valid_key(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (i == 0 && !ft_strchr(VAR, key[i]))
			return (0);
		if (i > 0 && !ft_strchr(MID, key[i]))
			return (0);
		i++;
	}
	return (1);
}
