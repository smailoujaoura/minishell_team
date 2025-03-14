/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:56:00 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/14 13:04:51 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Handle export command without args
void	print_keys_values(t_env *env)
{
	int		i;
	int		size;
	int		swap;
	char	*temp;
	t_env	*current;
	t_env	**sorted;

	size = ft_lstsize_env(env);
	if (size == 0)
		return ;
	sorted = ft_malloc(sizeof(t_env *) * (size + 1), ALLOCATE);
	sorted[size] = NULL;
	i = 0;
	current = env;
	while (i < size)
	{
		sorted[i] = current;
		current = current->next;
		i++;
	}
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
				temp = sorted[i]->key;
				sorted[i]->key = sorted[i + 1]->key;
				sorted[i + 1]->key = temp;
			}
			i++;
		}
	}

	while (env)
	{
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
		{
			printf("declare -x %s\n", env->key);
		}
		env = env->next;
	}
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

void	export_without_value(t_env *env, char *key)
{
	t_env	*ptr;
	t_env	*new;

	ptr = env;
	while (ptr)
	{
		if (ft_strncmp(ptr->key, key, SIZE_MAX) == 0)
			return ;
		ptr = ptr->next;
	}
	ptr = env;
	while (ptr->next)
		ptr = ptr->next;
	new = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
	new->key = ft_strdup(key, BKOLANI);
	new->next = NULL;
	new->value = NULL;
	ptr->next = new;
}

void	export_with_value(t_env *env, char **key_value, int action, int state)
{
	t_env	*ptr;
	t_env	*new;

	ptr = env;
	while (ptr)
	{
		if (ft_strncmp(key_value[0], ptr->key, SIZE_MAX) == 0)
		{
			state = 1;
			break ;
		}
		ptr = ptr->next;
	}
	if (action == UPDATE && state == 1)
	{
		ptr->value = ft_strjoin(ptr->value, key_value[1], BKOLANI);
	}
	else if (state)
	{
		ptr->value = ft_strdup(key_value[1], BKOLANI);
	}
	else if (action == UPDATE || action == CREATE)
	{
		new = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
		new->next = NULL;
		new->key = ft_strdup(key_value[0], BKOLANI);
		new->value = ft_strdup(key_value[1], BKOLANI);
		ft_lstadd_back_env(&env, new);
	}
}

int	check_args(t_env *env, char *line)
{
	char	**key_value;
	int		action;

	if (!*line || !ft_strchr(VAR, line[0]))
	{
		printf("minishell: export: `%s': not a valid identifier\n", line); // CHANGE to WRITE!
		return (1);
	}
	key_value = splitter(line, &action);
	if (key_value == NULL && !is_valid_key(line))
	{
		printf("minishell: export: `%s': not a valid identifier\n", line);
		return (1);
	}
	else if (key_value == NULL)
		export_without_value(env, line);
	else if (!is_valid_key(key_value[0]) || action == 0 || action == INVALID)
	{
		printf("minishell: export: `%s': not a valid identifier\n", line);
		return (1);
	}
	else
		export_with_value(env, key_value, action, 0);
	return (0);
}

// The main function to add an env var
int	builtin_export(t_env *env, char **argv, int flag)
{
	int	i;
	int	status;

	status = 0;
	if (!argv[1] && flag)
	{
		print_keys_values(env);
		return (status);
	}
	i = 0;
	while (argv[++i])
		status = check_args(env, argv[i]);
	return (status);
}
