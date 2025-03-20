/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:38 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/20 09:44:58 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_update_oldpwd(t_env *env, char *path)
{
	char	**updated_oldpwd;

	updated_oldpwd = ft_malloc_bkol(sizeof(char *) * 3, ALLOCATE);
	updated_oldpwd[0] = ft_strdup("export", BKOLANI);
	updated_oldpwd[1] = ft_strjoin("OLDPWD=", path, BKOLANI);
	updated_oldpwd[2] = NULL;
	builtin_export(env, updated_oldpwd, 0);
}

void	ft_update_pwd(t_env *env, char *path)
{
	char	**updated_pwd;

	updated_pwd = ft_malloc_bkol(sizeof(char *) * 3, ALLOCATE);
	updated_pwd[0] = ft_strdup("export", BKOLANI);
	updated_pwd[1] = ft_strjoin("PWD=", path, BKOLANI);
	updated_pwd[2] = NULL;
	builtin_export(env, updated_pwd, 0);
}

char	*store_pwd(char *appendage, int flag)
{
	static char	*pwd;
	char		*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		pwd = ft_strdup(cwd, BKOLANI);
		free(cwd);
		return (pwd);
	}
	else if (flag == STORE)
	{
		pwd = ft_strjoin(pwd, appendage, BKOLANI);
		return (pwd);
	}
	else
		return (pwd);
}

void	builtin_pwd(t_shell *mini)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		if (store_pwd(NULL, 2) == NULL)
		{
			printf("pwd: error retrieving current directory: getcwd: \
cannot access parent directories: %s\n", strerror(errno));
			mini->last_exit = 1;
		}
		else
			printf("%s\n", store_pwd(NULL, 2));
		return ;
	}
	printf("%s\n", path);
	free(path);
}
