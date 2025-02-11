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

void    create_files(t_chain *file)
{
  
	if (!file)
		return ;
	while (file)
	{
		if (open(file->file, O_CREAT, 0644) == -1)
			printf("Frees and exit\n");
		file = file->next;
	}
}

int   *create_pipe(void)
{
	int pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe()");
		exit(1);
	}
	return (pipe_fd);
}

// void create_process(t_ast *tree)
// {
//     pid_t pid;

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork()");
//         exit(1);
//     }
//     if (pid == 0)
//     {
//         // close(pipe_fd[READ_END]);
//         // dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
//         // close(pipe_fd[WRITE_END]);
//         // Exec
//     }
// }


void	config_streams(void)
{
	// this function will open up a bunch of files and ... returns two files descriptors in and out.
}

void	run_cmd(t_ast *tree, t_env *env)
{
	if (tree->data->empty)
		config_streams();
	else
	{
		if (check_buildin(tree->data))
			buildin_excutor(tree->data, env);
		else
		{
			config_streams();
			// find_path();
			// executre_cmd();
			// get exit status and assign it.
			tree->exit_status = 0;
		}
	}
}

void	run_pipe(t_ast *tree, t_env *env)
{
	int *pipe;

	tree->pipe = create_pipe();
	executor(tree->left, env);
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
		executor(tree->left, env);
		if (tree->left->exit_status == 0)
			executor(tree->right, env);
	}
	else if (tree->type == OR)
	{
		executor(tree->left, env);
		if (tree->left->exit_status != 0)
			executor(tree->right, env);
	}
	else
		printf("Something went wrong\n");
}

// void	run_and(t_ast *tree, t_env *env)
// {
	
// }

// void	run_or(t_ast *tree, t_env *env)
// {
	
// }