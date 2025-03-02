/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:05:37 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/02 09:00:50 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_path(char **argv, t_env *env)
{
	if (ft_strchr(argv[0], '/'))
		return (argv[0]);
	return (construct_cmd_path(argv, env));
}

void	open_redir(t_chain *redir)
{
	if (redir->ambiguous)
		return ;
	if (redir->type == REDIR_IN)
		redir->fd = open(redir->file, O_RDONLY);
	if (redir->type == REDIR_OUT)
		redir->fd = open(redir->file, O_WRONLY | O_CREAT, 0644);
	if (redir->type == REDIR_APPEND)
		redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->fd == -1)
	{
		dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("minishell: %s: %s\n", redir->file, strerror(errno));
	}
}

int	open_and_assign(t_chain *redirs)
{
	while (redirs)
	{
		open_redir(redirs);
		if (redirs->ambiguous || redirs->fd == -1)
		{
			return (1);
		}
		if (redirs->type == HEREDOC || redirs->type == REDIR_IN)
		{
			dup2(redirs->fd, STDIN_FILENO);
		}
		if (redirs->type == REDIR_APPEND || redirs->type == REDIR_OUT)
		{
			dup2(redirs->fd, STDOUT_FILENO);
		}
		close(redirs->fd);
		redirs = redirs->next;
	}
	return (0);
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
		return ("Command not found");
	return (error);
}

void	ext_proc(t_ast *tree, char **argv, char **envp, t_shell *mini)
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
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("%s%s: %s\n", get_string(1), argv[0], get_string(2));
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	exit(EXIT_FAILURE);
}

void	external_cmd(t_ast *tree, char **argv, char **envp, t_shell *mini)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		panic_exit("Forking a cmd for execve", 46);
	if (pid == 0)
		ext_proc(tree, argv, envp, mini);
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
