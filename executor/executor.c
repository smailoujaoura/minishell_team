#include "../minishell.h"

// # define READ_END 0
// # define WRITE_END 1

// Check if the file is executable 
char	*construct_cmd_path(const char *cmd, const char **envp)
{
	char *path;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	path = ft_strdup(envp[i] + 5, BKOLANI);
	printf("Env path: %s \n %s\n", path, cmd);
	return (NULL);
}


// int check_buildin(t_chain *data)
// {
// 	if (ft_strncmp("echo", data->content, ft_strlen(data->content)) == 0
// 		|| ft_strncmp("cd", data->content, ft_strlen(data->content)) == 0
// 		|| ft_strncmp("pwd", data->content, ft_strlen(data->content)) == 0
// 		|| ft_strncmp("export", data->content, ft_strlen(data->content)) == 0
// 		|| ft_strncmp("unset", data->content, ft_strlen(data->content)) == 0
// 		|| ft_strncmp("env", data->content, ft_strlen(data->content)) == 0
// 		|| ft_strncmp("exit", data->content, ft_strlen(data->content)) == 0)
// 		return (1);
// 	return (0);
// }

// void    buildin_excutor(t_chain *data, t_env *env_head)
// {
// 	if (ft_strncmp("echo", data->content, ft_strlen(data->content)) == 0)
// 		echo(data);
// 	if (ft_strncmp("cd", data->content, ft_strlen(data->content)) == 0)
// 		cd(env_head, data);
// 	if (ft_strncmp("pwd", data->content, ft_strlen(data->content)) == 0)
// 		pwd(data);
// 	if (ft_strncmp("export", data->content, ft_strlen(data->content)) == 0)
// 		export(env_head, data);
// 	if (ft_strncmp("unset", data->content, ft_strlen(data->content)) == 0)
// 		unset(env_head, data);
// 	if (ft_strncmp("env", data->content, ft_strlen(data->content)) == 0)
// 		mini_env(env_head, data);
// 	if (ft_strncmp("exit", data->content, ft_strlen(data->content)) == 0)
// 		mini_exit(data);
// }

// void	check_streams_perm(t_chain *file)
// {
// 	if (file->type == REDIR_IN)
// 	{
// 		if (access(file->file, F_OK | R_OK) == -1)
// 		{
// 			perror("access");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	else if (file->type == REDIR_OUT || file->type == REDIR_APPEND)
// 	{
// 		if (access(file->file, W_OK) == -1)
// 		{
// 			perror("access");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// }

// int   *create_pipe(void)
// {
// 	int pipe_fd[2];

// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("pipe()");
// 		exit(1);
// 	}
// 	return (pipe_fd);
// }

// void	config_streams(t_chain *adj, t_chain *blk, int *fd_in, int *fd_out)
// {
// 	t_chain	*adj_out;
// 	t_chain	*blk_out;
// 	t_chain	*adj_in;
// 	t_chain	*blk_in;

// 	// this function will open up a bunch of files and ... returns two files descriptors in and out.

// 	while (adj)
// 	{
		
// 		adj = adj->next;
// 	}
// 	while (blk)
// 	{
		
// 	}
// }

// void	run_cmd(t_ast *tree, t_env *env)
// {
// 	int	fd_in;
// 	int	fd_out;

// 	if (tree->data->empty)
// 		config_streams(tree->data->adj_f, tree->data->blk_f, &fd_in, &fd_out);
// 	else
// 	{
// 		if (check_buildin(tree->data))
// 			buildin_excutor(tree->data, env);
// 		else
// 		{
// 			// config_streams();
// 			// find_path();
// 			// executre_cmd();
// 			// get exit status and assign it.
// 			tree->exit_status = 0;
// 		}
// 	}
// }

// void	run_pipe(t_ast *tree, t_env *env)
// {
// 	int *pipe;

// 	tree->pipe = create_pipe();
// 	executor(tree->left, env);
// 	executor(tree->right, env);
// }

// void	executor(t_ast *tree, t_env *env)
// {
//     if (!tree)
//         return ;
//     if (tree->type == WORD)
//     {
//         if (tree->data->empty)
//         {
//             redir_out(tree->data->adj_f);
//             redir_out(tree->data->blk_f);
//         }
//         else
//         {
            
//         }
//     }
//     else if (tree->type == PIPE)
//     {

//     }
//     else if (tree->type == AND)
//         run_and();
//     else if (tree->type == OR)
//         run_or();
//     else
//         printf("Something went wrong\n");
// }


// void	run_or(t_ast *tree, t_env *env)
// {
	
// }
 void	executor(t_ast *tree, t_env *env)
 {
	while (tree)
	{
		tree = tree->parent;
	}
	while (env)
	{
		env = env->next;
	}
	return ;
 }
