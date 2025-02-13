#include "minishell.h"

# define READ_END 0
# define WRITE_END 1

int check_buildin(t_chain *data)
{
	if (ft_strncmp("echo", data->content, ft_strlen(data->content)) == 0
		|| ft_strncmp("cd", data->content, ft_strlen(data->content)) == 0
		|| ft_strncmp("pwd", data->content, ft_strlen(data->content)) == 0
		|| ft_strncmp("export", data->content, ft_strlen(data->content)) == 0
		|| ft_strncmp("unset", data->content, ft_strlen(data->content)) == 0
		|| ft_strncmp("env", data->content, ft_strlen(data->content)) == 0
		|| ft_strncmp("exit", data->content, ft_strlen(data->content)) == 0)
		return (1);
	return (0);
}

void    buildin_excutor(t_chain *data, t_env *env_head)
{
	if (ft_strncmp("echo", data->content, ft_strlen(data->content)) == 0)
		echo(data);
	if (ft_strncmp("cd", data->content, ft_strlen(data->content)) == 0)
		cd(env_head, data);
	if (ft_strncmp("pwd", data->content, ft_strlen(data->content)) == 0)
		pwd(data);
	if (ft_strncmp("export", data->content, ft_strlen(data->content)) == 0)
		export(env_head, data);
	if (ft_strncmp("unset", data->content, ft_strlen(data->content)) == 0)
		unset(env_head, data);
	if (ft_strncmp("env", data->content, ft_strlen(data->content)) == 0)
		mini_env(env_head, data);
	if (ft_strncmp("exit", data->content, ft_strlen(data->content)) == 0)
		mini_exit(data);
}

void	config_streams(t_chain *adj, t_chain *blk, int *fd_in, int *fd_out)
{
	t_chain	*adj_out;
	t_chain	*blk_out;
	t_chain	*adj_in;
	t_chain	*blk_in;

	// this function will open up a bunch of files and ... returns two files descriptors in and out.
	while (adj)
	{
		
	}
	while (blk)
	{
		
	}
}

/*
	1- check if cmd is empty in TYPE, then open streams and return.

	2- check variables(dollar) and expand them if yes into exp, the good news is that they are just args
	3- check if wild card then expand them, they are just args
	4- open heredocs and receive their inputs in hidden files 
	5- check IO files for existence and rights, starting from the lowest in depth and creating or opening if valid
			or breaking operation if not.
*/
void	run_cmd(t_ast *tree, t_env *env)
{
	int	fd_in;
	int	fd_out;

	if (tree->data->empty)
	{
		config_streams(tree->data->adj_f, tree->data->blk_f, &fd_in, &fd_out);
		// return
	}
	else
	{
		
	}
}

void	run_pipe(t_ast *tree, t_env *env)
{
	int *pipe;

	tree->pipe = create_pipe();
	executor(tree->left, env);
	executor(tree->right, env);
}

void	run_and(t_ast *tree, t_env *env)
{
	executor(tree->left, env);
	if (tree->left->exit_status == 0)
		executor(tree->right, env);
}

void	run_or(t_ast *tree, t_env *env)
{
	executor(tree->left, env);
	if (tree->left->exit_status != 0)
		executor(tree->right, env);
}

void	executor(t_ast *tree, t_env *env)
{
	if (!tree)
		return ;
	if (tree->type == WORD)
	{
		run_cmd(tree, env);
	}
	else if (tree->type == PIPE)
	{
		run_pipe(tree, env);
	}
	else if (tree->type == AND)
	{
		run_and(tree, env);
	}
	else if (tree->type == OR)
	{
		run_or(tree, env);
	}
	else
		printf("Something went wrong\n");
}
