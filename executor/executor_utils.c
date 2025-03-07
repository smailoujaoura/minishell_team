/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:05:37 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/06 22:49:29 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_path(char **argv, t_env *env)
{
	int		status;
	char	*path;

	status = is_a_dir(argv[0]);
	if (status != 0)
		exit(status);
	if (ft_strchr(argv[0], '/') || !get_value_wrapper("PATH", env)[0])
	{
		return (argv[0]);
	}
	path = construct_cmd_path(argv, env, -1);
	if (path == NULL)
	{
		ft_dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("%s: command not found\n", argv[0]);
		exit(127);
	}
	return (path);
}

char	*get_string(int which)
{
	char	*error;

	error = strerror(errno);
	if (which == 1)
	{
		if (ft_strnstr(error, "Bad", SIZE_MAX))
			return ("");
		return ("minishell: ");
	}
	if (ft_strnstr(error, "Bad", SIZE_MAX))
		return ("command not found");
	return (error);
}

void	ext_proc(t_ast *tree, char **argv, char **envp, t_shell *mini)
{
	char	*path;

	if (open_and_assign(tree->data->adj_f))
		exit(EXIT_FAILURE);
	if (tree->data->empty)
		exit(EXIT_SUCCESS);
	path = find_path(argv, mini->env);
	if (path == NULL)
		exit(127);
	if (tree->data->empty)
		exit(0);
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
		ext_proc(tree, argv, envp, mini);
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
