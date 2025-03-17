/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:02 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/17 15:59:55 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_err(char **av, t_env *env, char *path)
{
	if (!ft_strncmp(av[1], "..", SIZE_MAX))
		chdir("..");
	path = getcwd(NULL, 0);
	if (!path)
	{
		if (!ft_strncmp(av[1], "..", SIZE_MAX))
		{
			if (get_env_var(env, "PWD") && get_env_var(env, "OLDPWD"))
				ft_update_oldpwd(env, get_env_var(env, "PWD")->value);
			store_pwd("/..", -2);
		}
		else if (!ft_strncmp(av[1], ".", SIZE_MAX))
		{
			if (get_env_var(env, "PWD") && get_env_var(env, "OLDPWD"))
				ft_update_oldpwd(env, get_env_var(env, "PWD")->value);
			store_pwd("/.", -2);
		}
		write(2, "cd: error retrieving current directory: "
			"getcwd: cannot access parent directories: "
			"No such file or directory\n", 108);
		ft_update_pwd(env, store_pwd(NULL, 2));
	}
	free(path);
	return (1);
}

static int	cd_executor(t_env *env, char *cd_arg, int *status)
{
	char	*err_msg;
	char	*path;

	path = getcwd(NULL, 0);
	if (chdir(cd_arg) == -1)
	{
		free(path);
		write(2, "minishell: cd: ", 16);
		write(2, cd_arg, ft_strlen(cd_arg));
		write(2, ": ", 3);
		err_msg = strerror(errno);
		write(2, err_msg, ft_strlen(err_msg));
		write(2, "\n", 2);
		*status = 1;
		return (1);
	}
	ft_update_oldpwd(env, path);
	store_pwd(cd_arg, -3);
	free(path);
	path = NULL;
	*status = 0;
	return (0);
}

static void	cd_with_no_args(t_env *env, int *status)
{
	t_env	*home;
	char	*path;

	home = get_env_var(env, "HOME");
	if (!home || !home->value)
	{
		write(2, "minishell: cd: HOME not set\n", 29);
		*status = 1;
		return ;
	}
	if (!*home->value)
		return ;
	if (cd_executor(env, home->value, status))
		return ;
	path = getcwd(NULL, 0);
	ft_update_pwd(env, path);
	free(path);
}

static void	cd_with_args(t_env *env, char **argv, int *status)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path && handle_err(argv, env, NULL))
		return ;
	free(path);
	if (cd_executor(env, argv[1], status))
		return ;
	path = getcwd(NULL, 0);
	ft_update_pwd(env, path);
	free(path);
}

void	builtin_cd(t_env *env, char **argv, int *status)
{
	char	*pwd;

	if (argv[1] == NULL)
	{
		cd_with_no_args(env, status);
		pwd = getcwd(NULL, 0);
		store_pwd(pwd, -1);
		free(pwd);
	}
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
