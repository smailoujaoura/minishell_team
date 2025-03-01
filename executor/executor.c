/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/01 17:54:46 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	panic_exit(char *ptr, int place)
{
	(void)ptr;
	(void)place;
	perror("Minishell exit");
	exit(EXIT_FAILURE);
}

void	run_cmd(t_ast *tree, t_shell *mini)
{
	char	**argv;
	char	**envp;

	envp = generate_env_tab(mini->env);
	argv = expand_cmd(tree->data, tree->data->argv, mini);
	expand_redirs(tree->data->adj_f, mini);
	if (check_buildin(argv[0]))
		buildin_excutor(tree, argv, mini);
	else
		external_cmd(tree, argv, envp, mini);
	if (mini->last_exit == 131)
		printf("Quit (core dumped)\n");
}

void	run_pipe(t_ast *tree, t_shell *mini)
{
	int		pipe_pair[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(pipe_pair) == -1)
		panic_exit("Pipe wasn't created\n", 42);
	pid_left = fork();
	if (pid_left == 0)
		pipe_child(tree, mini, pipe_pair, 1);
	else if (pid_left == -1)
		panic_exit("Forking left", 43);
	pid_right = fork();
	if (pid_right == 0)
		pipe_child(tree, mini, pipe_pair, 0);
	if (pid_right == -1)
		panic_exit("Forking left", 44);
	close(pipe_pair[1]);
	close(pipe_pair[0]);
	wait(NULL);
	wait(NULL);
}

void	run_sub(t_ast *tree, t_shell *mini)
{
	pid_t	pid;

	expand_redirs(tree->data->adj_f, mini);
	pid = fork();
	if (pid == 0)
	{
		if (open_and_assign(tree->data->adj_f))
			exit(1);
		executor(tree->left, mini);
		wait(NULL);
		exit(0);
	}
	else if (pid == -1)
		panic_exit("Forking subshell", 45);
	wait(NULL);
}

void	executor(t_ast *tree, t_shell *mini)
{
	if (tree == NULL)
		return ;
	else if (tree->type == CMD)
	{
		run_cmd(tree, mini);
	}
	else if (tree->type == PIPE)
	{
		run_pipe(tree, mini);
	}
	else if (tree->type == SUB)
	{
		run_sub(tree, mini);
	}
	else if (tree->type == OR)
	{
		executor(tree->left, mini);
		printf("OR:[%d]\n", mini->last_exit);
		if (mini->last_exit != 0)
			executor(tree->right, mini);
	}
	else if (tree->type == AND)
	{
		executor(tree->left, mini);
		printf("AND:[%d]\n", mini->last_exit);
		if (mini->last_exit == 0)
			executor(tree->right, mini);
	}
}
