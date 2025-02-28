/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:05:37 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/27 22:51:15 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	external_cmd(t_ast *tree, char **argv, char **envp, t_shell *mini)
{
	char	*cmd_path;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		panic_exit("Forking a cmd for execve", 46);
	if (pid == 0)
	{
		if (tree->in_fd != -1)
		{
			dup2(tree->in_fd, STDIN_FILENO);
			close(tree->in_fd);
		}
		if (tree->out_fd != -1)
		{
			dup2(tree->out_fd, STDOUT_FILENO);
			close(tree->out_fd);
		}
		if (ft_strchr(argv[0], '/'))
			cmd_path = argv[0];
		else
			cmd_path = construct_cmd_path(argv, mini->env);
		if (execve(cmd_path, argv, envp) == -1)
				panic_exit(argv[0], 47);
	}
	wait(&mini->last_exit);
	mini->last_exit = WEXITSTATUS(mini->last_exit);
}

void	pipe_child(t_ast *tree, t_shell *mini, int *pipe_fd, int flag)
{
	if (!flag)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		executor(tree->left, mini);
		wait(NULL);
		exit(0);
	}
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		executor(tree->right, mini);
		wait(NULL);
		exit(0);
	}
}

void	fds_dup(t_ast *tree)
{
	if (tree->in_fd != -1)
	{
		dup2(tree->in_fd, STDIN_FILENO);
		close(tree->in_fd);
	}
	if (tree->out_fd != -1)
	{
		dup2(tree->out_fd, STDOUT_FILENO);
		close(tree->out_fd);
	}
}
