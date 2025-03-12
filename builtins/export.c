/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:56:00 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/12 21:53:40 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Handle export command without args
static void	export_with_no_args(t_env *env, char *line)
{
	t_env			*lst_env;

	lst_env = NULL;
	if (line)
	{
		lst_env = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
		lst_env->key = ft_strdup(line, BKOLANI);
		lst_env->value = NULL;
		lst_env->full = ft_strdup(line, BKOLANI);
		lst_env->next = NULL;
		ft_lstadd_back_env(&env, lst_env);
		return ;
	}
	while (env)
	{
		if (env->value)
			printf("declare -x %s=%c%s%c\n", env->key, '"', env->value, '"');
		else
			printf("declare -x %s\n", env->full);
		env = env->next;
	}
}

static void	process_env_var(t_env *env, char **str_tab, char *line, int action)
{
	t_env	*new_env;

	new_env = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
	if (action == UPDATE)
		new_env->key = str_tab[0];
	else
		new_env->key = ft_strdup(str_tab[0], BKOLANI);
	if (check_env(env, new_env->key) && action == UPDATE)
		update_env_concat(env, new_env, str_tab[1]);
	else if (check_env(env, new_env->key) && action == CREATE)
		update_env_trunc(env, new_env, line, str_tab[1]);
	else if (!check_env(env, new_env->key) && action == UPDATE)
		add_new_env_with_plus(env, new_env, str_tab[1]);
	else
		add_new_env(env, new_env, line, str_tab[1]);
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

int	check_export_env(t_env *env, char *line)
{
	char	**splited_line;
	int		action;

	if (!*line || !ft_strchr(VAR, line[0]))
	{
		printf("minishell: export: `%s': not a valid identifier\n", line);
		return (1);
	}
	if (is_valid_key(line) && !ft_strchr(line, '='))
		export_with_no_args(env, line);
	action = 0;
	splited_line = splitter(line, &action);
	if (splited_line == NULL)
	{
		if (!is_valid_key(line))
			printf("minishell: export: `%s': not a valid identifier\n", line);
		return (!is_valid_key(line));
	}
	if (!is_valid_key(splited_line[0]) || action == 0 || action == INVALID)
	{
		printf("minishell: export: `%s': not a valid identifier\n", line);
		return (1);
	}
	process_env_var(env, splited_line, line, action);
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
		export_with_no_args(env, NULL);
		return (status);
	}
	i = 0;
	while (argv[++i])
		status = check_export_env(env, argv[i]);
	return (status);
}
