/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/28 18:27:24 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	panic_exit(char *ptr, int place)
{
	printf("Reason: [%s]\tPlace [%d]\n", ptr, place);
	printf("%s: command not found\n", ptr);
	exit(EXIT_FAILURE);
}

void	run_cmd(t_ast *tree, t_shell *mini)
{
	char	**argv;
	char	**envp;
	bool	should_execute;

	argv = expand_cmd(tree->data, tree->data->argv, mini);
	envp = generate_env_tab(mini->env);
	expand_redirs(tree->data->adj_f, mini);
	should_execute = create_adj_files(tree->data->adj_f);
	if (tree->data->empty)
		mini->last_exit = 0;
	if (should_execute && !tree->data->empty)
	{
		assign_fds(tree);
		if (argv && check_buildin(argv[0]))
			buildin_excutor(tree, argv, mini);
		else if (argv)
			external_cmd(tree, argv, envp, mini);
	}
	else
		mini->last_exit = 1;
	// finish_exec(tree);
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
		pipe_child(tree, mini, pipe_pair, 0);
	else if (pid_left == -1)
		panic_exit("Forking left", 43);
	pid_right = fork();
	if (pid_right == 0)
		pipe_child(tree, mini, pipe_pair, 1);
	if (pid_right == -1)
		panic_exit("Forking left", 44);
	close(pipe_pair[1]);
	close(pipe_pair[0]);
	waitpid(-1, NULL, 0);
	waitpid(-1, NULL, 0);
}

void	run_sub(t_ast *tree, t_shell *mini)
{
	pid_t	pid;
	bool	should_execute;

	expand_redirs(tree->data->adj_f, mini);
	should_execute = create_adj_files(tree->data->adj_f);
	assign_fds(tree);
	if (should_execute)
	{
		pid = fork();
		if (pid == 0)
		{
			fds_dup(tree);
			executor(tree->left, mini);
			wait(NULL); // should get the exit status
			exit(0); // report the exit status
		}
		else if (pid == -1)
			panic_exit("Forking subshell", 45);
		wait(NULL);
	}
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
