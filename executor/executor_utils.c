/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:05:37 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/13 17:21:59 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	is_directory(char *cmd)
{
	struct stat	info;
	int			value;

	value = stat(cmd, &info);
	if ((ft_strlen(cmd) == 2 && ft_strncmp(cmd, "..", SIZE_MAX) == 0)
		|| (value == 0 && S_ISDIR(info.st_mode)))
	{
		write(2, "minishell: ..: Is a directory\n", 31);
		exit(126);
	}
	// else if (value == 0 && (access(cmd, X_OK) == -1 || !S_ISREG(info.st_mode)))
	// {
	// 	write(2, "minishell: ", 12);
	// 	write(2, cmd, ft_strlen(cmd));
	// 	write(2, ": ", 3);
	// 	write(2, strerror(errno), ft_strlen(strerror(errno)));
	// 	write(2, "\n", 2);
	// 	exit(126);
	// }
	// not needed unless the path is unset and will be handled after then.
}

char	*find_path(char **argv, t_env *env)
{
	char	*path;

	is_directory(argv[0]);
	if (ft_strchr(argv[0], '/') || !get_value_wrapper("PATH", env)[0])
	{
		return (argv[0]);
	}
	path = construct_cmd_path(argv, env, -1);
	if (path == NULL)
	{
		write(2, argv[0], ft_strlen(argv[0]));
		write(2, ": command not found\n", 21);
		exit(127);
	}
	return (path);
}

void	external_process(t_ast *tree, char **argv, char **envp, t_shell *mini)
{
	char	*path;

	if (open_and_assign(tree->data->adj_f))
		exit(1);
	if (tree->data->empty)
		exit(0);
	path = find_path(argv, mini->env);
	if (path == NULL)
		exit(127);
	if (execve(path, argv, envp) == -1)
	{
		if (access(path, X_OK) == 0)
		{
			exit(EXIT_SUCCESS);
		}
		else
		{
			printf("minishell: %s: %s\n", path, strerror(errno));
			exit(EXIT_FAILURE);
		}
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
		exit(mini->last_exit);
	}
	else
	{
		close(pipe_fd[1]);
		ft_dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		executor(tree->right, mini);
		wait(NULL);
		exit(mini->last_exit);
	}
}
