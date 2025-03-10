/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:02 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/10 14:48:08 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_err(t_env *env)
{
	char	*path;
	char	**argv;
	char	*new;

	chdir("..");
	path = getcwd(NULL, 0);
	if (!path)
	{
		write(2, "cd: error retrieving current directory: "
			"getcwd: cannot access parent directories: "
			"No such file or directory\n", 108);
		argv = ft_malloc_bkol(sizeof(char *) * 3, ALLOCATE);
		argv[0] = ft_strdup("export", BKOLANI);
		new = ft_strjoin("PWD=", get_value_wrapper("PWD", env), BKOLANI);
		new = ft_strjoin(new, "/..", BKOLANI);
		argv[1] = new;
		argv[2] = NULL;
		builtin_export(env, argv, 0);
		store_pwd(ft_strdup(new, BKOLANI), -1);
	}
	free(path);
	return (1);
}

static int	cd_executor(const char *cd_arg, char *path, int *status)
{
	if (chdir(cd_arg) == -1)
	{
		free(path);
		printf("minishell: cd: %s: %s\n", cd_arg, strerror(errno));
		*status = 1;
		return (1);
	}
	free(path);
	path = NULL;
	*status = 0;
	return (0);
}

static void	cd_with_no_args(t_env *env, int *status, char **old_pwd, char **pwd)
{
	t_env	*home;
	char	*path;

	old_pwd = ft_malloc_bkol(sizeof(char *) * 3, ALLOCATE);
	pwd = ft_malloc_bkol(sizeof(char *) * 3, ALLOCATE);
	home = get_env_var(env, "HOME");
	if (!home)
	{
		write(2, "minishell: cd: HOME not set\n", 29);
		return ;
	}
	path = getcwd(NULL, 0);
	old_pwd[0] = ft_strdup("export", BKOLANI);
	old_pwd[1] = ft_strjoin("OLDPWD=", path, BKOLANI);
	old_pwd[2] = NULL;
	builtin_export(env, old_pwd, 0);
	if (cd_executor(home->value, path, status))
		return ;
	path = getcwd(NULL, 0);
	pwd[0] = ft_strdup("export", BKOLANI);
	pwd[1] = ft_strjoin("PWD=", path, BKOLANI);
	pwd[2] = NULL;
	builtin_export(env, pwd, 0);
	free(path);
}

static void	cd_with_args(t_env *env, char **argv, int *status)
{
	char	*path;
	char	**updated_oldpwd;
	char	**updated_pwd;

	updated_oldpwd = ft_malloc_bkol(sizeof(char *) * 3, ALLOCATE);
	updated_pwd = ft_malloc_bkol(sizeof(char *) * 3, ALLOCATE);
	path = getcwd(NULL, 0);
	if (!path && handle_err(env))
		return ;
	updated_oldpwd[0] = ft_strdup("export", BKOLANI);
	updated_oldpwd[1] = ft_strjoin("OLDPWD=", path, BKOLANI);
	updated_oldpwd[2] = NULL;
	builtin_export(env, updated_oldpwd, 0);
	if (cd_executor(argv[1], path, status))
		return ;
	path = getcwd(NULL, 0);
	if (!path && handle_err(env))
		return ;
	updated_pwd[0] = ft_strdup("export", BKOLANI);
	updated_pwd[1] = ft_strjoin("PWD=", path, BKOLANI);
	updated_pwd[2] = NULL;
	builtin_export(env, updated_pwd, 0);
	free(path);
	return ;
}

void	builtin_cd(t_env *env, char **argv, int *status)
{
	if (argv[1] == NULL)
		cd_with_no_args(env, status, NULL, NULL);
	else
	{
		if (argv[2])
		{
			write(2, "minishell: cd: too many arguments\n", 35);
			*status = 1;
			return ;
		}
		cd_with_args(env, argv, status);
	}
}
