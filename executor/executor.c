/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/27 11:13:39 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	panic_exit(char *ptr, int place)
{
	printf("Reason: [%s]\tPlace [%d]\n", ptr, place);
	printf("%s: command not found\n", ptr);
	exit(EXIT_FAILURE);
}

void    buildin_excutor(t_ast *tree, char **argv, t_shell *mini)
{
	assign_fds_builtins(tree, 1);
	if (ft_strlen(argv[0]) == 4 && ft_strncmp("echo", argv[0], 4) == 0)
		builtin_echo(argv, &mini->last_exit);	
	if (ft_strlen(argv[0]) == 2 && ft_strncmp("cd", argv[0], 2) == 0)
		builtin_cd(mini->env, argv, &mini->last_exit);
	if (ft_strlen(argv[0]) == 3 && ft_strncmp("pwd", argv[0], 3) == 0)
		builtin_pwd();
	if (ft_strlen(argv[0]) == 6 && ft_strncmp("export", argv[0], 6) == 0)
		builtin_export(mini->env, argv, 1);
	if (ft_strlen(argv[0]) == 5 && ft_strncmp("unset", argv[0], 5) == 0)
		builtin_unset(mini->env, argv);
	if (ft_strlen(argv[0]) == 3 && ft_strncmp("env", argv[0], 3) == 0)
		builtin_env(mini->env, argv);	
	if (ft_strlen(argv[0]) == 4 && ft_strncmp("exit", argv[0], 4) == 0)
		builtin_exit(argv, &mini->last_exit);	
	assign_fds_builtins(tree, 0);
}

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

void	run_cmd(t_ast *tree, t_shell *mini)
{
	char	**argv;
	char	**envp;
	bool	should_execute;

	argv = expand_cmd(tree->data, tree->data->argv, mini->env);
	envp = generate_env_tab(mini->env);
	expand_redirs(tree->data->adj_f, mini);
	should_execute = create_adj_files(tree->data->adj_f);
	if (tree->data->empty)
		mini->last_exit = 0;
	if (should_execute && !tree->data->empty)
	{
		assign_fds(tree);
		if (check_buildin(argv[0]))
			buildin_excutor(tree, argv, mini);
		else
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
	{
		close(pipe_pair[0]);
		dup2(pipe_pair[1], STDOUT_FILENO);
		close(pipe_pair[1]);
		executor(tree->left, mini);
		wait(NULL);
		exit(0);
	}
	else if (pid_left == -1)
		panic_exit("Forking left", 43);
	pid_right = fork();
	if (pid_right == 0)
	{
		close(pipe_pair[1]);
		dup2(pipe_pair[0], STDIN_FILENO);
		close(pipe_pair[0]);
		executor(tree->right, mini);
		wait(NULL);
		exit(0);
	}
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
		run_cmd(tree, mini);
	else if (tree->type == PIPE)
		run_pipe(tree, mini);
	else if (tree->type == SUB)
		run_sub(tree, mini);
	else if (tree->type == OR)
	{
		executor(tree->left, mini);
		if (mini->last_exit != 0)
			executor(tree->right, mini);
	}
	else if (tree->type == AND)
	{
		executor(tree->left, mini);
		if (mini->last_exit == 0)
			executor(tree->right, mini);
	}
	else
		printf("Minishell: Parse again\n");
}
