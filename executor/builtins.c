#include "../minishell.h"

// int builtins_redir_fd(t_chain *file)
// {
//     int fd;

//     if (file->type == REDIR_IN)
//         fd = open(file->file, O_RDONLY, 0644);
//     if (file->type == REDIR_APPEND)
//         fd = open(file->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
//     else if (file->type == REDIR_OUT)
//         fd = open(file->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
//     {
//         perror("open");
//         exit(EXIT_FAILURE);
//     }
//     return (fd);
// }

// void    builtins_redir(t_chain *redir_file, char *buff, int create_only)
// {
//     int fd;

//     if (create_only)
//     {
//         while (redir_file)
//         {
//             fd = builtins_redir_fd(redir_file);
//             close(fd);
//             redir_file = redir_file->next;
//         }
//         return ;
//     }
//     while (redir_file)
//     {
//         fd = builtins_redir_fd(redir_file);
//         if (!redir_file->next)
//             break;
//         close(fd);
//         redir_file = redir_file->next;
//     }
//     if (redir_file->type == REDIR_OUT 
//         || redir_file->type == REDIR_APPEND)
//         write(fd, buff, ft_strlen(buff));
// }

// void    redir_output(t_chain *data, char *output)
// {
//     if (data->adj_f)
//     {
//         builtins_redir(data->adj_f, output, 0);
//         if (data->blk_f)
//             builtins_redir(data->blk_f, NULL, 1);
//     }
//     else if (data->blk_f)
//         builtins_redir(data->blk_f, output, 0);
//     else
//         write(1, output, ft_strlen(output));
// }

// // exit
// void    create_files_only(t_chain *data)
// {
//     if (data->adj_f)
//         builtins_redir(data->adj_f, NULL, 1);
//     if (data->blk_f)
//         builtins_redir(data->blk_f, NULL, 1);
// }

void    check_exit_status(char *str, int *status)
{
    int i;

    i = -1;
    while (str[++i])
    {
        if (str[i] < '0' && str[i] > 9)
        {
            printf("exit: %s: numeric argument required\n", str);
            *status = 2;
            exit(2);
        }
    }
}

void    builtin_exit(char **argv, int *status)
{
    int exit_status;

    write(1, "exit\n", 5);
    if (!argv[1])
    {
        *status = 0;
        exit(EXIT_SUCCESS);
    }
    if (argv[2])
    {
        write(2, "exit: too many arguments\n", 25);
        *status = 1;
        return ;
    }
    check_exit_status(argv[1], status);
    exit_status = ft_atoi(argv[1]);
    exit_status = exit_status % 256;
    *status = exit_status;
    exit(exit_status);
}

// echo
// static void    check_option(t_argv **argv, int *option_n)
// {
//     int i;

//     i = 2;
//     while ((*argv)->content[i] && (*argv)->content[i] == 'n')
//         i++;
//     if ((*argv)->content[i] == '\0')
//     {
//         (*option_n)++;
//         *argv = (*argv)->next;
//     }
// }

int	skip_rest(char *arg, int *new_line)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != 'n')
		{
			break ;
			return (1);
		}
	}
	*new_line = 0;
	return (0);
}

void	print_status(char **argv, int *status)
{
	if (argv[1] && ft_strlen(argv[1]) == 2 && !ft_strncmp(argv[1], "$?", 2))
	{
		printf("%d\n", *status);
		*status = 0;
	}
}

void	builtin_echo(char **argv, int *status)
{
	int	new_line;
	int	i;

	new_line = 1;
	i = 1;
	if (argv[1] == NULL)
		return ;
	print_status(argv, status);
	while (argv[i])
	{
		if (argv[i][0] == '-' && argv[i][1] == 'n')
			skip_rest(argv[i], &new_line);
		else
			break ;
		i++;
	}
    i = 1;
	while (argv[i])
	{
		printf("%s", argv[i++]);
		if (argv[i] != NULL)
			printf(" ");
	}
	if (new_line)
		printf("\n");
}

// void echo(t_chain *data, t_env *env, int *gl_stat)
// {
//     int option_n = 0;
// 	t_env	*exp_env;

//     if (!data->argv)
//         return;
//     if (!data->argv->next && ft_strlen(data->argv->content) == 2 
//         && ft_strncmp("$?", data->argv->content, 2) == 0)
//     {
//         printf("%d\n", *gl_stat);
//         *gl_stat = 0;
//         return ;
//     }
//     if (data->argv->content[0] == '-' && data->argv->content[1] == 'n')
//         check_option(&data->argv, &option_n);
//     while (data->argv)
//     {
//         if (option_n && data->argv->content[0] == '-' && data->argv->content[1] == 'n')
//             check_option(&data->argv, &option_n);
//         if (data->argv->content[0] == '$' && data->argv->content[1])
// 		{
// 			exp_env = get_env_var(env, data->argv->content + 1);
// 			if (exp_env)
// 				write(1, exp_env->value, ft_strlen(exp_env->value));
// 		}
//         else if (data->argv->content[0] != '#')
//             write(1, data->argv->content, ft_strlen(data->argv->content));
//         else if (data->argv->content[0] == '#')
//             break ;
//         data->argv = data->argv->next;
//         if (data->argv)
//             write(1, " ", 1);
//     }
//     if (!option_n)
//         write(1, "\n", 1);
//     *gl_stat = 0;
// }

// cd
static  int cd_executor(const char *cd_arg, char *path, int *status)
{
    if (chdir(cd_arg) == -1)
    { 
        free(path);
        perror("cd");
        *status = 1;
        return (1);
    }
    free(path);
    path = NULL;
    return (0);
}

static void    cd_with_no_args(t_env *env, int *status)
{
    t_env   *home;
    char    *path;
    char    **updated_oldpwd;
    char    **updated_pwd;

    updated_oldpwd = ft_malloc_bkol(sizeof(char *) * 2, ALLOCATE);
    updated_pwd = ft_malloc_bkol(sizeof(char *) * 2, ALLOCATE);
    home = get_env_var(env, "HOME");
    path = getcwd(NULL, 0);
    updated_oldpwd[0] = ft_strjoin("OLDPWD=", path, BKOLANI);
    updated_oldpwd[1] = NULL;
    builtin_export(env, updated_oldpwd);
    if (cd_executor(home->value, path, status))
        return ;
    path = getcwd(NULL, 0);
    updated_pwd[0] = ft_strjoin("PWD=", path, BKOLANI);
    updated_pwd[1] = NULL;
    builtin_export(env, updated_pwd);
    free(path);
}

static void    cd_with_args(t_env *env, char **argv, int *status)
{
    char    *path;
    char    **updated_oldpwd;
    char    **updated_pwd;

    updated_oldpwd = ft_malloc_bkol(sizeof(char *) * 2, ALLOCATE);
    updated_pwd = ft_malloc_bkol(sizeof(char *) * 2, ALLOCATE);
    path = getcwd(NULL, 0);
    updated_oldpwd[0] = ft_strjoin("OLDPWD=", path, BKOLANI);
    updated_oldpwd[1] = NULL;
    builtin_export(env, updated_oldpwd);
    if (cd_executor(argv[1], path, status))
        return ;
    path = getcwd(NULL, 0);
    updated_pwd[0] = ft_strjoin("PWD=", path, BKOLANI);
    updated_pwd[1] = NULL;
    builtin_export(env, updated_pwd);
    free(path);
    return ;
}

void    builtin_cd(t_env *env, char **argv, int *status)
{
    if (argv[1] == NULL)
        cd_with_no_args(env, status);
    else
    {
        if (argv[2])
        {
            write(2, "cd: too many arguments\n", 23);
            *status = 1;
            return ;
        }
        cd_with_args(env, argv, status);
    }
}

// pwd
void    builtin_pwd(void)
{
    char *path;

    path = getcwd(NULL, 0);
    if (!path)
    {
        perror("pwd");
        return ;
    }
    printf("%s\n", path);
    free(path);
}
