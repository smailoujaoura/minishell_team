/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:07:14 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/17 10:32:44 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	**splitter_helper(char *str, int i, int *action, char **array)
{
	if (str[i] == '+' && str[i + 1] == '=')
	{
		array[0] = ft_substr(str, 0, i, SOUJAOUR);
		array[1] = ft_substr(str, i + 2, ft_strlen(str) - (i + 2), SOUJAOUR);
		*action = UPDATE;
		return (array);
	}
	else if (str[i] == '=')
	{
		array[0] = ft_substr(str, 0, i, SOUJAOUR);
		array[1] = ft_substr(str, i + 1, ft_strlen(str) - (i + 1), SOUJAOUR);
		*action = CREATE;
		return (array);
	}
	else
	{
		*action = INVALID;
		return (NULL);
	}
}

char	**splitter(char *str, int *action)
{
	int		i;
	char	**array;

	i = 0;
	*action = 0;
	array = ft_malloc(sizeof(char *) * 3, SOUJAOUR);
	array[2] = NULL;
	while (str[i])
	{
		if ((str[i] == '=' || str[i] == '+'))
		{
			return (splitter_helper(str, i, action, array));
		}
		i++;
	}
	return (NULL);
}

static t_env	*create_new_env(char *line)
{
	char	**splited_line;
	t_env	*new_env;
	int		action;

	new_env = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
	splited_line = splitter(line, &action);
	new_env->key = ft_strdup(splited_line[0], BKOLANI);
	new_env->value = ft_strdup(splited_line[1], BKOLANI);
	new_env->next = NULL;
	return (new_env);
}

t_env	*handle_env(char **envp, int i, char *cwd)
{
	t_env	*head;
	t_env	*new;

	head = NULL;
	cwd = getcwd(NULL, 0);
	while (envp[++i])
	{
		if (ft_strncmp("PWD=", envp[i], 5) == 0
			|| ft_strncmp("OLDPWD=", envp[i], 8) == 0)
		{
			if (cwd)
			{
				new = create_new_env(ft_strjoin("PWD=", cwd, BKOLANI));
				ft_lstadd_back_env(&head, new);
				new = create_new_env(ft_strjoin("OLDPWD=", cwd, BKOLANI));
				ft_lstadd_back_env(&head, new);
			}
			continue ;
		}
		new = create_new_env(envp[i]);
		ft_lstadd_back_env(&head, new);
	}
	if (cwd)
		free(cwd);
	return (head);
}
