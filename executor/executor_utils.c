/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:05:37 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/17 11:02:45 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	is_directory(char *cmd, t_env *env, int value, struct stat info)
{
	value = stat(cmd, &info);
	if (ft_strlen(cmd) == 2 && ft_strncmp(cmd, "..", SIZE_MAX) == 0)
	{
		if (expand_env_var(env, "PATH") == NULL
			|| expand_env_var(env, "PATH")[0] == '\0')
		{
			write(2, "minishell: ", 12);
			write(2, cmd, ft_strlen(cmd));
			write(2, ": Is a directory\n", 18);
			ft_exit(126);
		}
		else
		{
			write(2, "..: command not found\n", 23);
			ft_exit(127);
		}
	}
	else if (value == 0 && S_ISDIR(info.st_mode))
	{
		write(2, "minishell: ", 12);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Is a directory\n", 18);
		ft_exit(126);
	}
}

char	*find_path(char **argv, t_env *env)
{
	char		*path;
	struct stat	info;
	int			value;

	value = stat(argv[0], &info);
	is_directory(argv[0], env, value, info);
	if (ft_strchr(argv[0], '/')
		|| !get_value_wrapper("PATH", env)
		|| !get_value_wrapper("PATH", env)[0])
		return (argv[0]);
	if (!ft_strncmp(get_value_wrapper("PATH", env), ".", SIZE_MAX))
	{
		if (access(argv[0], F_OK) == 0)
			return (ft_strjoin("./", argv[0], BKOLANI));
	}
	path = construct_cmd_path(argv, env, -1);
	if (path == NULL)
	{
		write(2, argv[0], ft_strlen(argv[0]));
		write(2, ": command not found\n", 21);
		ft_exit(127);
	}
	return (path);
}

void	external_process(t_ast *tree, char **argv, char **envp, t_shell *mini)
{
	char	*path;

	if (open_and_assign(tree->data->adj_f))
		ft_exit(1);
	if (tree->data->empty)
		ft_exit(0);
	path = find_path(argv, mini->env);
	if (path == NULL)
		ft_exit(127);
	if (execve(path, argv, envp) == -1)
	{
		write(2, "minishell: ", 12);
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
		path = strerror(errno);
		write(2, path, ft_strlen(path));
		write(2, "\n", 2);
		if (errno == ENOENT)
			ft_exit(127);
		else if (errno == EACCES)
			ft_exit(126);
		else
			ft_exit(1);
	}
}

void	external_cmd(t_ast *tree, char **argv, char **envp, t_shell *mini)
{
	pid_t	pid;

	pid = ft_fork();
	if (pid == 0)
	{
		setup_signals(3);
		external_process(tree, argv, envp, mini);
	}
	waitpid(pid, &mini->last_exit, WUNTRACED);
	if (WIFEXITED(mini->last_exit))
		mini->last_exit = WEXITSTATUS(mini->last_exit);
	else if (WIFSIGNALED(mini->last_exit))
		mini->last_exit = WTERMSIG(mini->last_exit) + 128;
}

void	pipe_child(t_ast *tree, t_shell *mini, int *pipe_fd, int flag)
{
	if (flag)
	{
		close(pipe_fd[0]);
		ft_dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		executor(tree->left, mini);
		wait(NULL);
		ft_exit(mini->last_exit);
	}
	else
	{
		close(pipe_fd[1]);
		ft_dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		executor(tree->right, mini);
		wait(NULL);
		ft_exit(mini->last_exit);
	}
}
