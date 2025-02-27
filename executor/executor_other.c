/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_other.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/27 11:11:34 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**generate_env_tab(t_env *envp)
{
	int		i;
	char	**env;
	t_env	*tmp;

	i = 0;
	tmp = envp;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	env = ft_malloc_bkol((sizeof(char *) * (i + 1)), ALLOCATE);
	i = -1;
	while (envp)
	{
		env[++i] = ft_strdup(envp->full, BKOLANI);
		envp = envp->next;
	}
	env[i] = NULL;
	return (env);
}

char	*construct_cmd_path(char **argv, t_env *envp)
{
	char	*path;
	int		i;
	char	**spl_path;
	char	*tmp_cmd;
	char	*new_cmd;

	i = -1;
	tmp_cmd = NULL;
	while (envp)
	{
		if (ft_strncmp(envp->full, "PATH=", 5) == 0)
			break ;
		envp = envp->next;
	}
	path = ft_strdup(envp->full + 5, BKOLANI);
	spl_path = ft_split(path, ':', BKOLANI);
	while (spl_path[++i])
	{
		new_cmd = ft_strjoin("/", argv[0], BKOLANI);
		tmp_cmd = ft_strjoin(spl_path[i], new_cmd, BKOLANI);
		if (access(tmp_cmd, X_OK) == 0)
			return (tmp_cmd);
	}
	return (NULL);
}

int	check_buildin(const char *cmd)
{
	if ((ft_strlen(cmd) == 4 && ft_strncmp("echo", cmd, 4) == 0)
		|| (ft_strlen(cmd) == 2 && ft_strncmp("cd", cmd, 2) == 0)
		|| (ft_strlen(cmd) == 3 && ft_strncmp("pwd", cmd, 3) == 0)
		|| (ft_strlen(cmd) == 6 && ft_strncmp("export", cmd, 6) == 0)
		|| (ft_strlen(cmd) == 5 && ft_strncmp("unset", cmd, 5) == 0)
		|| (ft_strlen(cmd) == 3 && ft_strncmp("env", cmd, 3) == 0)
		|| (ft_strlen(cmd) == 4 && ft_strncmp("exit", cmd, 4) == 0))
		return (1);
	return (0);
}

