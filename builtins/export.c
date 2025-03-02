/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:56:00 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/28 17:23:12 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// For each case use the rigth function
static void	process_env_var(t_env *env, t_env *new_env, char **str_tab,
	const char *line)
{
	if (str_tab[0][ft_strlen(str_tab[0]) - 1] == '+')
		new_env->key = ft_substr(str_tab[0], 0, ft_strlen(str_tab[0]) - 1,
				BKOLANI);
	else
		new_env->key = ft_strdup(str_tab[0], BKOLANI);
	if (check_env(env, new_env->key)
		&& str_tab[0][ft_strlen(str_tab[0]) - 1] == '+')
		update_env_concat(env, new_env, str_tab[1]);
	else if (check_env(env, new_env->key)
		&& str_tab[0][ft_strlen(str_tab[0]) - 1] != '+')
		update_env_trunc(env, new_env, line, str_tab[1]);
	else if (!check_env(env, new_env->key)
		&& str_tab[0][ft_strlen(str_tab[0]) - 1] == '+')
		add_new_env_with_plus(env, new_env, str_tab[1]);
	else
		add_new_env(env, new_env, line, str_tab[1]);
}

// Handle export command without args
static void	export_with_no_args(t_env *env)
{
	while (env)
	{
		printf("declare -x %s\n", env->full);
		env = env->next;
	}
}

static void	check_export_env(t_env *env, char *line)
{
	char	**splited_line;
	t_env	*new_env;

	if (!ft_strchr(line, '=') && line[ft_strlen(line) -1] == '+')
	{
		printf("minishell: export: `%s': not a valid identifier\n", line);
		return ;
	}
	else if (!ft_strchr(line, '='))
		return ;
	new_env = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
	splited_line = ft_split(line, '=', BKOLANI);
	if (check_env_str(line, splited_line))
	{
		ft_malloc_bkol(0, DEALLOCATE);
		return ;
	}
	process_env_var(env, new_env, splited_line, line);
}

// The main function to add an env var
void	builtin_export(t_env *env, char **argv, int flag)
{
	int	i;

	if (!argv[1] && flag)
	{
		export_with_no_args(env);
		return ;
	}
	i = -1;
	while (argv[++i])
		check_export_env(env, argv[i]);
}
