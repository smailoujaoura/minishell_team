#include "../minishell.h"

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
		init_process(tree, argv[0], argv, env_tab);
	else
	{
		cmd_path = construct_cmd_path(argv, env);
		if (!cmd_path)
		{
			write(2, "minishell: command not found\n", 29);
			tree->exit_status = 127;
			return ;
		}
		init_process(tree, cmd_path, argv, env_tab);
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


void	create_adj_files(t_chain *adj)
{
	// t_chain	*ptr;

	// ptr = adj;
	(void)adj;
	// while (ptr)
	// {
	// 	if (ptr->type != HEREDOC)
	// 	{
	// 		if (ptr->type == REDIR_OUT)
	// 		{
	// 			ptr->fd = ft_open(ptr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// 			if (ptr->fd == -1)
	// 				return ;
	// 		}
	// 	}
	// 	ptr = ptr->next;
	// }
}


int run_empty_cmd(t_ast *tree, t_env *env)
{
	expand_redirs(tree->data->adj_f, NULL, env);
	create_adj_files(tree->data->adj_f);
	return (0);
}


void	run_cmd(t_ast *tree, t_env *env)
{
	char			**argv;
	char			**envp;

	if (tree->data->empty)
	{
		run_empty_cmd(tree, env);
		return ;
	}
	argv = expand_cmd(tree->data, tree->data->argv, env);
	envp = generate_env_tab(env);
	create_adj_files(tree->data->adj_f);
	(void)envp;
	(void)argv;
	
	// // if ((tree->parent && tree->parent->type == PIPE) || !check_buildin(argv[0]))
	// // {
	// // 	create_proc_if(tree, env, argv, envp);
	// // 	status = tree->exit_status;
	// // }
	// else
	// {
	// 	buildin_excutor(argv, env, &status);
	// 	tree->exit_status = status;
	// }
}

void	run_pipe(t_ast *tree)
{
	int	pipe_pair[2];

	if (pipe(pipe_pair) == -1)
	{
		perror("minishell");
		return ;
	}
	(void)tree;
	(void)pipe_pair;
}

void	executor(t_ast *tree, t_env *env)
{
	if (tree == NULL)
		return ;
	(void)env;
	// if (tree->type == WORD)
	// {
	// 	run_cmd(tree, env);
	// 	executor(tree->left, env);
	// 	executor(tree->right, env);
	// }
	// if (tree->type == PIPE)
	// {
	// 	run_pipe(tree);
	// 	executor(tree->left, env);
	// 	executor(tree->right, env);
	// }
	// if (tree->type == AND)
	// {
	// 	executor(tree->left, env);
	// 	if (tree->left->exit_status == 0)
	// 		executor(tree->right, env);
	// }
	// if (tree->type == OR)
	// {
	// 	executor(tree->left, env);
	// 	if (tree->left->exit_status != 0)
	// 		executor(tree->right, env);
	// }
	
}
