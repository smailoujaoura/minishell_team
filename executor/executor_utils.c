/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:05:37 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/01 16:24:54 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

# define APPENDING_MODE O_WRONLY | O_CREAT | O_APPEND

char	*find_path(char **argv, t_env *env)
{
	if (ft_strchr(argv[0], '/'))
		return (argv[0]);
	return (construct_cmd_path(argv, env));
}

int	open_and_assign(t_chain *redirs)
{
	while (redirs)
	{
		if (redirs->ambiguous)
			return (1);
		if (redirs->type == REDIR_IN)
		{
			redirs->fd = open(redirs->file, O_RDONLY);
			dup2(redirs->fd, STDIN_FILENO);
		}
		else if (redirs->type == REDIR_OUT)
		{
			redirs->fd = open(redirs->file, O_WRONLY | O_CREAT, 0644);
			dup2(redirs->fd, STDOUT_FILENO);
		}
		else if (redirs->type == REDIR_APPEND)
		{
			redirs->fd = open(redirs->file, APPENDING_MODE, 0644);
			dup2(redirs->fd, STDOUT_FILENO);
		}
		else
			dup2(redirs->fd, STDOUT_FILENO);
		if (redirs->fd == -1)
			return (1);
		close(redirs->fd);
		redirs = redirs->next;
	}
	return (0);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
}

void	external_cmd(t_ast *tree, char **argv, char **envp, t_shell *mini)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		panic_exit("Forking a cmd for execve", 46);
	if (pid == 0)
	{
		if (open_and_assign(tree->data->adj_f))
			exit(1);
		if (tree->data->empty)
		{
			close(STDOUT_FILENO);
			close(STDIN_FILENO);
			exit(0);
		}
		setup_child_signals();
		execve(find_path(argv, mini->env), argv, envp);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &mini->last_exit, WUNTRACED);
	if (WIFEXITED(mini->last_exit))
		mini->last_exit = WEXITSTATUS(mini->last_exit);
	if (WIFSIGNALED(mini->last_exit))
		mini->last_exit = WTERMSIG(mini->last_exit) + 128;
}

void	pipe_child(t_ast *tree, t_shell *mini, int *pipe_fd, int flag)
{
	if (flag)
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
