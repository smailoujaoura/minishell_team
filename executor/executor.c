#include "../minishell.h"

// # define READ_END 0
// # define WRITE_END 1

// char	**generate_args_tab(t_chain *data, t_env *env)
// {
// 	int		i;
// 	char	**argv;
// 	t_argv	*tmp;
// 	t_env	*exp_env;

// 	i = 0;
// 	tmp = data->argv;
// 	while (tmp)
// 	{
// 		i++;
// 		tmp = tmp->next;
// 	}
// 	argv = ft_malloc_bkol(sizeof(char *) * (i + 2), ALLOCATE);
// 	i = -1;
// 	tmp = data->argv;
// 	argv[++i] = ft_strdup(data->content, BKOLANI);
// 	while (tmp)
// 	{
// 		if (tmp->content[0] == '$' && tmp->content[1] != '?')
// 		{
// 			exp_env = get_env_var(env, tmp->content + 1);
// 			if (exp_env)
// 				argv[++i] = ft_strdup(exp_env->value, BKOLANI);
// 		}
// 		else
// 			argv[++i] = ft_strdup(tmp->content, BKOLANI);
// 		tmp = tmp->next;
// 	}
// 	argv[++i] = NULL;
// 	// printf("Generate here: %s\n", argv[1]);
// 	return (argv);
// }

char	**generate_env_tab(t_env *envp)
{
	int		i;
	char	**env;
	t_env	*tmp;

	i = 0;
	tmp = envp;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	env = ft_malloc_bkol((sizeof(char *) * (i + 1)), ALLOCATE);
	i = -1;
	while (envp)
	{
		env[++i] = ft_strdup(envp->full, BKOLANI);
		envp = envp->next;
	}
	env[i] = NULL;
	return (env);
}

void	init_process(t_ast *tree, const char *cmd_path, char *argv[], char *envp[])
{
	pid_t	pid;
	int		proc_stat;

	pid = fork();
	if (pid == -1)
	{
		tree->exit_status = EXIT_FAILURE;
		return ;
	}
	if (pid == 0)
	{
		if (tree->parent == PIPE)
		{
			close(tree->parent->pipe[0]);
			dup2(tree->parent->pipe[1], STDOUT_FILENO);
			close(tree->parent->pipe[1]);
		}
		if (execve(cmd_path, argv, envp) == -1)
		{
			perror("minishell");
			exit(127);
		}
	}
	wait(&proc_stat);
	tree->exit_status = WEXITSTATUS(proc_stat);
	return ;
}

char	*construct_cmd_path(char **argv, t_env *envp)
{
	char *path;
	int		i;
	char	**spl_path;
	char	*tmp_cmd;
	char	*new_cmd;

	i = -1;
	tmp_cmd = NULL;
	while (envp)
	{
		if (ft_strncmp(envp->full, "PATH=", 5) == 0)
			break ;
		envp = envp->next;
	}
	path = ft_strdup(envp->full + 5, BKOLANI);
	spl_path = ft_split(path, ':', BKOLANI);
	while (spl_path[++i])
	{
		new_cmd = ft_strjoin("/", argv[0], BKOLANI);
		tmp_cmd = ft_strjoin(spl_path[i], new_cmd, BKOLANI);
		if (access(tmp_cmd, X_OK) == 0)
			return (tmp_cmd);
	}
	return (NULL);
}

int check_buildin(const char *cmd)
{
	if (ft_strncmp("echo", cmd, ft_strlen(cmd)) == 0
		|| ft_strncmp("cd", cmd, ft_strlen(cmd)) == 0
		|| ft_strncmp("pwd", cmd, ft_strlen(cmd)) == 0
		|| ft_strncmp("export", cmd, ft_strlen(cmd)) == 0
		|| ft_strncmp("unset", cmd, ft_strlen(cmd)) == 0
		|| ft_strncmp("env", cmd, ft_strlen(cmd)) == 0
		|| ft_strncmp("exit", cmd, ft_strlen(cmd)) == 0)
		return (1);
	return (0);
}

void    buildin_excutor(char **argv, t_env *env_head, int *glob_st)
{
	if (ft_strncmp("echo", argv[0], ft_strlen(argv[0])) == 0)
		builtin_echo(argv, glob_st);
	if (ft_strncmp("cd", argv[0], ft_strlen(argv[0])) == 0)
		builtin_cd(env_head, argv, glob_st);
	if (ft_strncmp("pwd", argv[0], ft_strlen(argv[0])) == 0)
		builtin_pwd();
	if (ft_strncmp("export", argv[0], ft_strlen(argv[0])) == 0)
		builtin_export(env_head, argv);
	if (ft_strncmp("unset", argv[0], ft_strlen(argv[0])) == 0)
		builtin_unset(env_head, argv);
	if (ft_strncmp("env", argv[0], ft_strlen(argv[0])) == 0)
		builtin_env(env_head, argv);
	if (ft_strncmp("exit", argv[0], ft_strlen(argv[0])) == 0)
		builtin_exit(argv, glob_st);
}

void	external_cmd(t_ast *tree, t_env	*env, char **argv, char **env_tab)
{
	char	*cmd_path;

	if (ft_strchr(argv[0], '/'))
		init_process(argv[0], argv, env_tab, tree);
	else
	{
		cmd_path = construct_cmd_path(argv, env);
		if (!cmd_path)
		{
			write(2, "minishell: command not found\n", 29);
			tree->exit_status = 127;
			return ;
		}
		init_process(cmd_path, argv, env_tab, &tree->exit_status);
	}
}

int	ft_open(char *path, int mode, int permissions)
{
	int fd;

	fd = open(path, mode, permissions);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", path, strerror(errno));
	}
	return (fd);
}

void	create_blk_files(t_chain *blk, int deepest)
{
	int		current;
	t_chain	*ptr;

	current = 0;
	while (current <= deepest)
	{
		ptr = blk;
		while (ptr)
		{
			if (ptr->type != HEREDOC && ptr->depth == current)
			{
				if (ptr->type == REDIR_OUT)
				{
					ptr->fd = ft_open(ptr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (ptr->fd == -1)
						return ;
				}
			}
			ptr = ptr->next;
		}
		current++;
	}
}

void	create_adj_files(t_chain *adj)
{
	t_chain	*ptr;

	ptr = adj;
	while (ptr)
	{
		if (ptr->type != HEREDOC)
		{
			if (ptr->type == REDIR_OUT)
			{
				ptr->fd = ft_open(ptr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (ptr->fd == -1)
					return ;
			}
		}
		ptr = ptr->next;
	}
}

int find_deepest(t_chain *ptr)
{
	int deepest = -1;

	while (ptr)
	{
		if (ptr->depth > deepest)
			deepest = ptr->depth;
		ptr = ptr->next;
	}
	return (deepest);
}

int run_empty_cmd(t_ast *tree, t_env *env)
{
	expand_redirs(tree->data->adj_f, tree->data->blk_f, env);
	create_blk_files(tree->data->blk_f, find_deepest(tree->data->blk_f));
	create_adj_files(tree->data->adj_f);
	return (0);
}

void	create_proc_if(t_ast *tree, t_env *env, char **argv, char **envp)
{
	external_cmd(tree, env, argv, envp);
}

void	run_cmd(t_ast *tree, t_env *env)
{
	char			**argv;
	char			**envp;
	static int		status;

	if (tree->data->empty)
	{
		run_empty_cmd(tree, env);
		return ;
	}
	argv = expand_cmd(tree->data, tree->data->argv, env);
	envp = generate_env_tab(env);
	create_blk_files(tree->data->blk_f, find_deepest(tree->data->blk_f));
	create_adj_files(tree->data->adj_f);
	
	

	if (tree->parent == PIPE || !check_buildin(argv[0]))
	{
		create_proc_if(tree, env, argv, envp);
		status = tree->exit_status;
	}
	else
	{
		buildin_excutor(argv, env, &status);
		tree->exit_status = status;
	}
}

void	run_pipe(t_ast *tree)
{
	if (pipe(tree->pipe) == -1)
	{
		perror("minishell");
		return ;
	}
}

void	executor(t_ast *tree, t_env *env)
{
	if (tree == NULL)
		return ;
	if (tree->type == WORD)
	{
		run_cmd(tree, env);
	}
	if (tree->type == PIPE)
	{
		run_pipe(tree);
	}
	if (tree->type == AND)
	{
		executor(tree->left, env);
		if (tree->left->exit_status == 0)
			executor(tree->right, env);
	}
	if (tree->type == OR)
	{
		executor(tree->left, env);
		if (tree->left->exit_status != 0)
			executor(tree->right, env);
	}
}
