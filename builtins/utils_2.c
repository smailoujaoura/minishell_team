/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:07:14 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/05 21:16:35 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	*create_new_env(const char *line)
{
	char	**splited_line;
	t_env	*new_env;

	new_env = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
	splited_line = ft_split(line, '=', BKOLANI);
	new_env->key = ft_strdup(splited_line[0], BKOLANI);
	if (splited_line[1])
		new_env->value = ft_strdup(splited_line[1], BKOLANI);
	new_env->full = ft_strdup(line, BKOLANI);
	new_env->next = NULL;
	return (new_env);
}

t_env	*handle_env(char **envp)
{
	int		i;
	t_env	*head;
	t_env	*new;

	i = -1;
	head = NULL;
	while (envp[++i])
	{
		new = create_new_env(envp[i]);
		ft_lstadd_back_env(&head, new);
	}
	return (head);
}

t_env	*get_env_var(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, SIZE_MAX) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	check_env_str(const char *line)
{
	int	i;
	char **splited_str;

	i = -1;
	if (!ft_strchr(VAR, line[++i]))
	{
		printf("Minishell: export: `%s': not a valid identifier\n", line);
		return (1);
	}
	splited_str = ft_split(line, '=', BKOLANI);
	while (splited_str[0][++i])
	{
		if (!ft_strchr(MID, splited_str[0][i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n", line);
			return (1);
		}
	}
	return (0);
}
