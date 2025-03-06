/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/06 17:37:31 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	panic_exit(char *ptr, int custom)
{
	if (custom)
	{
		printf("%s", ptr);
	}
	else
		perror("Minishell exit");
	ft_malloc(0, DEALLOCATE);
	ft_malloc_bkol(0, DEALLOCATE);
	exit(custom);
}

int	run_sub(t_ast *tree, t_shell *mini, t_chain *files, pid_t pid)
{
	expand_redirs(tree->data->adj_f, mini);
	pid = ft_fork();
	if (pid == 0)
	{
		if (open_and_assign(tree->data->adj_f))
			exit(1);
		tree->left->f = 1;
		executor(tree->left, mini);
		exit(mini->last_exit);
	}
	while (files)
	{
		if (files->ambiguous)
			break ;
		if (files->type == HEREDOC)
			close(files->fd);
		files = files->next;
	}
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
	waitpid(pid_right, &mini->last_exit, WUNTRACED);
	if (WIFEXITED(mini->last_exit))
		mini->last_exit = WEXITSTATUS(mini->last_exit);
	else if (WIFSIGNALED(mini->last_exit))
		mini->last_exit = WTERMSIG(mini->last_exit) + 128;
}

void	run_cmd(t_ast *tree, t_shell *mini, char **argv)
{
	char		**envp;
	t_chain		*ptr;

	if (!argv)
		return ;
	if (is_a_dir(argv[0], mini) || is_empty_cmd(argv[0], mini))
		return ;
	envp = generate_env_tab(mini->env);
	if (check_buildin(argv[0]))
		buildin_excutor(tree, argv, mini);
	else
		external_cmd(tree, argv, envp, mini);
	printf("%d\n", mini->last_exit);
	if (mini->last_exit == 131)
		printf("Quit (core dumped)\n");
	if (mini->last_exit == 130)
		write(1, "\n", 1);
	ptr = tree->data->adj_f;
	while (ptr)
	{
		if (ptr->ambiguous)
			break ;
		if (ptr->type == HEREDOC)
			close(ptr->fd);
		ptr = ptr->next;
	}
}

void	executor(t_ast *tree, t_shell *mini)
{
	if (tree == NULL)
		return ;
	else if (tree->type == CMD)
	{
		expand_redirs(tree->data->adj_f, mini);
		run_cmd(tree, mini, expand_cmd(tree->data, tree->data->argv, mini));
	}
	else if (tree->type == PIPE)
		run_pipe(tree, mini);
	else if (tree->type == SUB)
		mini->last_exit = run_sub(tree, mini, tree->data->adj_f, -2);
	else if (tree->type == OR)
	{
		((tree->f == 1) && (tree->left->f = 1) && (tree->right->f = 1));
		executor(tree->left, mini);
		if (mini->last_exit != 0)
			executor(tree->right, mini);
	}
	else if (tree->type == AND)
	{
		((tree->f == 1) && (tree->left->f = 1) && (tree->right->f = 1));
		executor(tree->left, mini);
		if (mini->last_exit == 0)
			executor(tree->right, mini);
	}
}
