/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/25 22:42:12 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	panic_exit(char *ptr, int place)
{
	printf("Reason: [%s]\tPlace [%d]\n", ptr, place);
	printf("%s: command not found\n", ptr);
	exit(EXIT_FAILURE);
}

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
		// else
		// 	perror("2: ");
	}
	return (NULL);
}

int check_buildin(const char *cmd)
{
	
	if (ft_strncmp("echo", cmd, ft_strlen("echo")) == 0
		|| ft_strncmp("cd", cmd, ft_strlen("cd")) == 0
		|| ft_strncmp("pwd", cmd, ft_strlen("pwd")) == 0
		|| ft_strncmp("export", cmd, ft_strlen("export")) == 0
		|| ft_strncmp("unset", cmd, ft_strlen("unset")) == 0
		|| ft_strncmp("env", cmd, ft_strlen("env")) == 0
		|| ft_strncmp("exit", cmd, ft_strlen("exit")) == 0)
		return (1);
	return (0);
}

void    buildin_excutor(char **argv, t_shell *mini)
{
	if (ft_strncmp("echo", argv[0], ft_strlen("echo")) == 0)
		builtin_echo(argv, &mini->last_exit);
	if (ft_strncmp("cd", argv[0], ft_strlen("cd")) == 0)
		builtin_cd(mini->env, argv, &mini->last_exit);
	if (ft_strncmp("pwd", argv[0], ft_strlen("pwd")) == 0)
		builtin_pwd();
	if (ft_strncmp("export", argv[0], ft_strlen("export")) == 0)
		builtin_export(mini->env, argv);
	if (ft_strncmp("unset", argv[0], ft_strlen("unset")) == 0)
		builtin_unset(mini->env, argv);
	if (ft_strncmp("env", argv[0], ft_strlen("env")) == 0)
		builtin_env(mini->env, argv);
	if (ft_strncmp("exit", argv[0], ft_strlen("exit")) == 0)
		builtin_exit(argv, &mini->last_exit);
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


bool	create_adj_files(t_chain *adj)
{
	t_chain	*ptr;

	ptr = adj;
	while (ptr)
	{
		if (ptr->ambiguous)
			break ;
		if (ptr->type != HEREDOC)
		{
			if (ptr->type == REDIR_OUT)
				ptr->fd = ft_open(ptr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (ptr->type == REDIR_APPEND)
				ptr->fd = ft_open(ptr->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else if (ptr->type == REDIR_IN)
				ptr->fd = ft_open(ptr->file, O_RDONLY , 0644);
		}
		if (ptr->fd == -1)
			return (false);
		ptr = ptr->next;
	}
	return (true);
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

void	assign_fds(t_ast *tree)
{
	t_chain	*ptr;

	ptr = tree->data->adj_f;
	while (ptr)
	{
		if (ptr->type == REDIR_APPEND || ptr->type == REDIR_OUT)
			tree->out_fd = ptr->fd;
		if (ptr->type == REDIR_IN)
			tree->in_fd = ptr->fd;
		ptr = ptr->next;
	}
}

void	run_cmd(t_ast *tree, t_shell *mini)
{
	char	**argv;
	char	**envp;
	bool	should_execute;

	argv = expand_cmd(tree->data, tree->data->argv, mini->env);
	envp = generate_env_tab(mini->env);
	expand_redirs(tree->data->adj_f, mini->env);
	should_execute = create_adj_files(tree->data->adj_f);
	if (tree->data->empty)
		mini->last_exit = 0;
	if (should_execute && !tree->data->empty)
	{
		assign_fds(tree);
		if (check_buildin(argv[0]))
			buildin_excutor(argv, mini);
		else
			external_cmd(tree, argv, envp, mini);
	}
	else
		mini->last_exit = 1;
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

	expand_redirs(tree->data->adj_f, mini->env);
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
