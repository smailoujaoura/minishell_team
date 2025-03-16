/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:56:10 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/16 16:28:25 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// env maker
char	**make_env(char **envp)
{
	char	**env;
	char	*cwd;
	char	*path;

	env = ft_malloc_bkol((sizeof(char *) * 4), ALLOCATE);
	if (envp && envp[0])
		return (envp);
	cwd = getcwd(NULL, 0);
	path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	env[0] = ft_strjoin("PWD=", ft_strdup(cwd, BKOLANI), BKOLANI);
	env[1] = ft_strjoin("PATH=", path, BKOLANI);
	env[2] = ft_strjoin("OLDPWD=", ft_strdup(cwd, BKOLANI), BKOLANI);
	env[3] = NULL;
	free(cwd);
	return (env);
}

void	init_shell(char **envp, t_shell *mini)
{
	char			*pwd;

	envp = make_env(envp);
	mini->env = handle_env(envp);
	mini->last_exit = 0;
	mini->volatile_exit = 0;
	mini->flag = 0;
	mini->export = 0;
	mini->doubles = 0;
	mini->singles = 0;
	mini->num = 1;
	pwd = getcwd(NULL, 0);
	if (pwd)
		store_pwd(ft_strdup(pwd, BKOLANI), -1);
	else
	{
		perror("shell-init: error retrieving current directory: getcwd: \
cannot access parent directories");
	}
	free(pwd);
}

// Print env vars
void	builtin_env(t_env *env)
{
	if (!env)
		return ;
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
