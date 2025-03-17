/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:56:10 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/17 10:33:02 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// env maker
char	**make_env(char **envp)
{
	char	**env;
	char	*cwd;
	char	*path;

	path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	env = ft_malloc_bkol((sizeof(char *) * 4), ALLOCATE);
	if (envp && envp[0])
		return (envp);
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		env[0] = ft_strjoin("PWD=", ft_strdup(cwd, BKOLANI), BKOLANI);
		env[2] = ft_strjoin("OLDPWD=", ft_strdup(cwd, BKOLANI), BKOLANI);
		free(cwd);
	}
	env[1] = ft_strjoin("PATH=", path, BKOLANI);
	env[3] = NULL;
	return (env);
}

void	init_shell(char **envp, t_shell *mini)
{
	char			*pwd;

	envp = make_env(envp);
	mini->env = handle_env(envp, -1, NULL);
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
