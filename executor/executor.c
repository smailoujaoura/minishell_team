#include "../minishell.h"

// # define READ_END 0
// # define WRITE_END 1

static int	ft_lstsize_wldc(t_argv *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		size++;
		lst = lst->next;
	}
	return (size);
}

// Handle a wild card as command
void	wild_card_as_cmd()
{

}

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
}

int	wild_card_syntax_Handler(const char *wild_card)
{
	int	i;

	if (!ft_strchr(wild_card, '*'))
		return (1);
	i = 0;
	while (wild_card[i] == ' ' || wild_card[i] == '\t')
		i++;
	while (wild_card[i])
	{
		if (wild_card[i] == '<' || wild_card[i] == '>')
		{
			i++;
			while (wild_card[i] == '*')
				i++;
			if (wild_card[i] == '\0')
			{
				printf("minishell: %s: ambiguous redirect\n", (wild_card + 1));
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}

void	wild_card_handler(t_ast *node, const char **envp)
{
	char	*path;
	DIR *dir;
	struct dirent *entry;
	int	i;
	int time;
	// char	*pattern;
	const char *wild_card;
	char	*cmd;
	const char **cmd_arg;

	wild_card = node->data->content;
	if(wild_card_syntax_Handler(wild_card))
		return ;
	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("getcwd failed");
		exit(EXIT_FAILURE);
	}
	dir = opendir(path);
	if (!dir)
	{
		perror("opendir failed");
		exit(EXIT_FAILURE);
	}
	i = 0;
	time = 0;
	// If the wild card has not a parent so try to execute
	// the first entry of the directory as a command
	if (!node->parent)
	{
		if (ft_strncmp(wild_card, "*", ft_strlen(wild_card)) == 0)
		{
			while ((entry = readdir(dir)))
			{
				if (entry->d_name[0] != '.')
					break ;
			}
			cmd = construct_cmd_path(wild_card, envp);
			if (node->data->argv)
			{
				i = ft_lstsize_wldc(node->data->argv);
				cmd_arg = ft_malloc_bkol(((sizeof(char *) * i) + 1), ALLOCATE);
				i = 0;
				while (node->data->argv)
				{
					cmd_arg = ft_strdup(node->data->argv->content, BKOLANI);
					i++;
					node->data->argv = node->data->argv->next;
				}
			}
			// if (execve(entry->d_name, (char *const []){NULL}, (char *const *)envp) == -1)
			// {
			// 	// printf("minishell: command not found: %s\n", entry->d_name);
			// 	perror("execve");
			// 	closedir(dir);
			// 	free(path);
			// 	return ;
			// }
		}
	}
	i = 0;
	while (wild_card[i])
	{
		if (wild_card[i] == '*' && (wild_card[i + 1] == ' ' || wild_card[i + 1] == '\0'))
			time++;
		if (wild_card[i] != '*' && wild_card[i] != ' ')
		{
			// 
		}
		if (wild_card[i] == '\0')
			break ;
		i++;
	}
	while ((entry = readdir(dir)))
	{
		i =  0;
		while (++i <= time)
			printf("%s\n", entry->d_name);
	}
	if (closedir(dir) == -1)
	{
		perror("closedir failed");
		exit(EXIT_FAILURE);
	}
	free(path);
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

// void	run_and(t_ast *tree, t_env *env)
// {
	
// }

// void	run_or(t_ast *tree, t_env *env)
// {
	
// }
 