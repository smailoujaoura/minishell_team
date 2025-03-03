/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_other.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/03 16:03:22 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	map_errno_to_exit_status(void)
{
	if (errno == ENOENT)
		return (127);
	if (errno == ENOEXEC || errno == EACCES || errno == EISDIR)
		return (126);
	return (1);
}

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
	i = 0;
	while (envp)
	{
		env[i] = ft_strdup(envp->full, BKOLANI);
		envp = envp->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

char	*construct_cmd_path(char **argv, t_env *envp, int i)
{
	char	*path;
	char	**spl_path;
	char	*tmp_cmd;
	char	*new_cmd;

	tmp_cmd = NULL;
	while (envp)
	{
		if (ft_strncmp(envp->full, "PATH=", 5) == 0)
			break ;
		envp = envp->next;
	}
	if (!envp)
		return (NULL);
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
	if (ft_strncmp("echo", cmd, SIZE_MAX) == 0
		|| ft_strncmp("cd", cmd, SIZE_MAX) == 0
		|| ft_strncmp("pwd", cmd, SIZE_MAX) == 0
		|| ft_strncmp("export", cmd, SIZE_MAX) == 0
		|| ft_strncmp("unset", cmd, SIZE_MAX) == 0
		|| ft_strncmp("env", cmd, SIZE_MAX) == 0
		|| ft_strncmp("exit", cmd, SIZE_MAX) == 0)
		return (1);
	return (0);
}

void	buildin_excutor(t_ast *tree, char **argv, t_shell *mini)
{
	if (assign_fds_builtins(tree, argv[0], 1))
		return ;
	if (ft_strncmp("echo", argv[0], SIZE_MAX) == 0)
		builtin_echo(argv, &mini->last_exit);
	if (ft_strncmp("cd", argv[0], SIZE_MAX) == 0)
		builtin_cd(mini->env, argv, &mini->last_exit);
	if (ft_strncmp("pwd", argv[0], SIZE_MAX) == 0)
		builtin_pwd(mini);
	if (ft_strncmp("export", argv[0], SIZE_MAX) == 0)
		builtin_export(mini->env, argv, 1);
	if (ft_strncmp("unset", argv[0], SIZE_MAX) == 0)
		builtin_unset(mini->env, argv);
	if (ft_strncmp("env", argv[0], SIZE_MAX) == 0)
		builtin_env(mini->env, argv);
	if (ft_strncmp("exit", argv[0], SIZE_MAX) == 0)
	{
		if (isatty(STDOUT_FILENO) && isatty(STDIN_FILENO) && tree->f == 0)
			printf("exit\n");
		builtin_exit(argv, &mini->last_exit);
	}
	assign_fds_builtins(tree, argv[0], 0);
}
