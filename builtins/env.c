/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:56:10 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/05 17:53:22 by soujaour         ###   ########.fr       */
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
	if (envp[0])
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

// Print env vars
void	builtin_env(t_env *env, char **argv)
{
	if (!env)
		return ;
	if (argv[1])
	{
		printf("env: '%s': No such file or directory\n", argv[1]);
		return ;
	}
	while (env)
	{
		printf("%s\n", env->full);
		env = env->next;
	}
}
