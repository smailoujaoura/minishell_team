/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:56:00 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/07 10:07:45 by soujaour         ###   ########.fr       */
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

char	**splitter(char *str)
{
	int		i;
	char	**array;

	i = 0;
	array = ft_malloc(sizeof(char *) * 3, SOUJAOUR);
	array[2] = NULL;
	while (str[i])
	{
		if (str[i] == '=')
		{
			array[0] = ft_substr(str, 0, i, BKOLANI);
			array[1] = ft_substr(str, i + 1, ft_strlen(str) - i, BKOLANI);
			return (array);
		}
		i++;
	}
	return (NULL);
}

static void	check_export_env(t_env *env, char *line)
{
	char	**splited_line;
	t_env	*new_env;

	if (!*line || !ft_strchr(VAR, line[0]))
	{
		printf("minishell: export: `%s': not a valid identifier\n", line);
		return ;
	}
	if (!ft_strchr(line, '=') && check_env_str(line))
		return ;
	new_env = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
	splited_line = ft_split(line, '=', BKOLANI);
	// splited_line = splitter(line);
	if (check_env_str(splited_line[0]))
		return ;
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
	i = 0;
	while (argv[++i])
		check_export_env(env, argv[i]);
}


// export var"=""'='one'+'two"
// export var='='one'+'two

// export var'='one'+'two
// export var'='one'+'two