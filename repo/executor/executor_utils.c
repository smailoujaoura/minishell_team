/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:05:37 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/03 16:02:52 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_path(char **argv, t_env *env)
{
	if (ft_strchr(argv[0], '/') || !get_value_wrapper(argv[0], env)[0])
		return (argv[0]);
	return (construct_cmd_path(argv, env, -1));
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
	if (open_and_assign(tree->data->adj_f))
		exit(EXIT_FAILURE);
	if (tree->data->empty)
	{
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		exit(0);
	}
	setup_child_signals();
	execve(find_path(argv, mini->env), argv, envp);
	if (ft_strnstr(strerror(errno), "Exec format", SIZE_MAX))
		exit(0);
	if (dup2(STDERR_FILENO, STDOUT_FILENO) < 0)
	{
		perror("minishell");
		return ;
	}
	printf("%s%s: %s\n", get_string(1), argv[0], get_string(2));
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	exit(map_errno_to_exit_status());
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
		if (ft_dup2(pipe_fd[1], STDOUT_FILENO))
			return ;
		close(pipe_fd[1]);
		executor(tree->left, mini);
		wait(NULL);
		exit(mini->last_exit);
	}
	else
	{
		close(pipe_fd[1]);
		if (ft_dup2(pipe_fd[0], STDIN_FILENO))
			return ;
		close(pipe_fd[0]);
		executor(tree->right, mini);
		wait(NULL);
		exit(mini->last_exit);
	}
}
