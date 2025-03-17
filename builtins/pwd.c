/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:38 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/17 13:12:38 by bkolani          ###   ########.fr       */
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

char	*store_pwd(char *store_it, int flag)
{
	static char	*pwd;
	char		*cur;

	cur = getcwd(NULL, 0);
	if (cur)
	{
		pwd = ft_strdup(cur, BKOLANI);
		return (free(cur), pwd);
	}
	if (flag == -1)
		pwd = ft_strdup(store_it, BKOLANI);
	else if (flag == -2)
		pwd = ft_strjoin(pwd, store_it, BKOLANI);
	else if (flag == -3)
	{
		if (store_it[ft_strlen(store_it) - 1] == '/')
			store_it = ft_substr(store_it, 0,
					(ft_strlen(store_it) - 1), BKOLANI);
		if (store_it[0] != '/')
			pwd = ft_strjoin(pwd, ft_strjoin("/", store_it, BKOLANI), BKOLANI);
	}
	if (pwd)
		return (pwd);
	else
		return (ft_strdup("/home/soujaour", BKOLANI));
}

void	builtin_pwd(t_shell *mini)
{
	char	*path;
	t_env	*pwd_str;

	path = getcwd(NULL, 0);
	pwd_str = NULL;
	if (!path)
	{
		pwd_str = get_env_var(mini->env, "PWD");
		if (pwd_str == NULL)
			printf("%s\n", store_pwd(NULL, 2));
		else if (pwd_str)
			printf("%s\n", pwd_str->value);
		return ;
	}
	printf("%s\n", path);
	free(path);
}
