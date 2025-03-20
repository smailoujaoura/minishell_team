/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:07:14 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/20 09:59:17 by soujaour         ###   ########.fr       */
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
	if (line == NULL)
		return (NULL);
	splited_line = splitter(line, &action);
	if (splited_line == NULL)
		return (NULL);
	if (splited_line[0])
		new_env->key = ft_strdup(splited_line[0], BKOLANI);
	else
		new_env->key = splited_line[0];
	if (splited_line[1])
		new_env->value = ft_strdup(splited_line[1], BKOLANI);
	else
		new_env->value = splited_line[1];
	new_env->next = NULL;
	return (new_env);
}

t_env	*handle_env(char **envp, int i, char *cwd, t_shell *mini)
{
	static char	*argv1[3] = {"export", "OLDPWD", NULL};
	static char	*argv2[3] = {"unset", "OLDPWD", NULL};
	t_env		*head;
	char		*path;

	head = NULL;
	mini->env = head;
	path = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	if (envp == NULL || envp[0] == NULL)
		ft_lstadd_back_env(&head, create_new_env(path));
	else
	{
		while (envp && envp[++i])
			ft_lstadd_back_env(&head, create_new_env(envp[i]));
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_update_pwd(head, cwd);
		free(cwd);
	}
	builtin_unset(mini, argv2);
	builtin_export(head, argv1, 0);
	return (head);
}
