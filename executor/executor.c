/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/02 20:21:28 by soujaour         ###   ########.fr       */
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
	t_chain	*ptr;

	envp = generate_env_tab(mini->env);
	argv = expand_cmd(tree->data, tree->data->argv, mini);
	expand_redirs(tree->data->adj_f, mini);
	if (check_buildin(argv[0]))
		buildin_excutor(tree, argv, mini);
	else
		external_cmd(tree, argv, envp, mini);
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

int	run_sub(t_ast *tree, t_shell *mini, t_chain *files)
{
	pid_t	pid;

	expand_redirs(tree->data->adj_f, mini);
	pid = fork();
	if (pid == 0)
	{
		if (open_and_assign(tree->data->adj_f))
			exit(1);
		executor(tree->left, mini);
		exit(mini->last_exit);
	}
	else if (pid == -1)
		panic_exit("Forking subshell", 45);
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

// walk the tree recursively and execute each node as needed
void	executor(t_ast *tree, t_shell *mini)
{
	if (tree == NULL)
		return ;
	else if (tree->type == CMD)
		run_cmd(tree, mini);
	else if (tree->type == PIPE)
		run_pipe(tree, mini);
	else if (tree->type == SUB)
		mini->last_exit = run_sub(tree, mini, tree->data->adj_f);
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
}
