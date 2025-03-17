/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/17 09:53:02 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_heredoc(t_chain *files)
{
	while (files)
	{
		if (files->ambiguous)
			break ;
		if (files->type == HEREDOC)
			close(files->fd);
		files = files->next;
	}
}

int	run_sub(t_ast *tree, t_shell *mini, t_chain *files, pid_t pid)
{
	expand_redirs(tree->data->adj_f, mini);
	pid = ft_fork();
	if (pid == 0)
	{
		if (open_and_assign(tree->data->adj_f))
			exit(1);
		tree->left->sub = 1;
		executor(tree->left, mini);
		ft_exit(mini->last_exit);
	}
	close_heredoc(files);
	waitpid(pid, &mini->last_exit, WUNTRACED);
	if (WIFEXITED(mini->last_exit))
		return (WEXITSTATUS(mini->last_exit));
	return (1);
}

void	run_pipe(t_ast *tree, t_shell *mini)
{
	int		pipe_pair[2];
	pid_t	pid_left;
	pid_t	pid_right;

	ft_pipe(pipe_pair);
	pid_left = fork();
	if (pid_left == 0)
		pipe_child(tree, mini, pipe_pair, 1);
	pid_right = fork();
	if (pid_right == 0)
		pipe_child(tree, mini, pipe_pair, 0);
	close(pipe_pair[1]);
	close(pipe_pair[0]);
	waitpid(pid_left, NULL, WUNTRACED);
	waitpid(pid_right, &mini->last_exit, WUNTRACED);
	if (WIFEXITED(mini->last_exit))
		mini->last_exit = WEXITSTATUS(mini->last_exit);
	else if (WIFSIGNALED(mini->last_exit))
		mini->last_exit = WTERMSIG(mini->last_exit) + 128;
	if (tree->left)
		close_heredoc(tree->left->data->adj_f);
	if (tree->right)
		close_heredoc(tree->right->data->adj_f);
}

void	run_cmd(t_ast *tree, t_shell *mini, char **argv)
{
	char		**envp;

	if (is_empty_command(argv, mini))
		return ;
	envp = generate_env_tab(mini->env);
	if (check_builtin(argv[0]))
		execute_builtin(tree, argv, mini);
	else
		external_cmd(tree, argv, envp, mini);
	if (mini->last_exit == 131)
		write(2, "Quit (core dumped)\n", 20);
	if (mini->last_exit == 130)
		write(1, "\n", 1);
	close_heredoc(tree->data->adj_f);
}

void	executor(t_ast *tree, t_shell *mini)
{
	if (tree == NULL || mini->volatile_exit == 130)
		return ;
	else if (tree->type == CMD)
	{
		expand_redirs(tree->data->adj_f, mini);
		run_cmd(tree, mini, expand_cmd(tree->data, tree->data->argv, mini));
		mini->volatile_exit = mini->last_exit;
	}
	else if (tree->type == PIPE)
	{
		run_pipe(tree, mini);
		mini->volatile_exit = mini->last_exit;
	}
	else if (tree->type == SUB)
	{
		mini->last_exit = run_sub(tree, mini, tree->data->adj_f, -2);
		mini->volatile_exit = mini->last_exit;
	}
	else if (tree->type == OR)
		run_or(tree, mini);
	else if (tree->type == AND)
		run_and(tree, mini);
}
