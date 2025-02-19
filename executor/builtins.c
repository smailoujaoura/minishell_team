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

void    mini_exit(t_chain *data, int *status)
{
    int i;
    int exit_status;

    write(1, "exit\n", 5);
    if (!data->argv || !data->argv->content)
        exit(EXIT_SUCCESS);
    if (data->argv->next)
    {
        write(2, "exit: too many arguments\n", 25);
        *status = 1;
        return ;
    }
    i = -1;
    while (data->argv->content[++i])
    {
        if (data->argv->content[i] < '0' && data->argv->content[i] > 9)
        {
            printf("exit: %s: numeric argument required\n", data->argv->content);
            *status = 2;
            exit(2);
        }
    }
    exit_status = ft_atoi(data->argv->content);
    exit_status = exit_status % 256;
    *status = exit_status;
    exit(exit_status);
}

// echo
static void    check_option(t_argv **argv, int *option_n)
{
    int i;

    i = 2;
    while ((*argv)->content[i] && (*argv)->content[i] == 'n')
        i++;
    if ((*argv)->content[i] == '\0')
    {
        (*option_n)++;
        *argv = (*argv)->next;
    }
}

void echo(t_chain *data, t_env *env, int *gl_stat)
{
    int option_n = 0;
	t_env	*exp_env;

    if (!data->argv)
        return;
    if (!data->argv->next && ft_strlen(data->argv->content) == 2 
        && ft_strncmp("$?", data->argv->content, 2) == 0)
    {
        printf("%d\n", *gl_stat);
        *gl_stat = 0;
        return ;
    }
    if (data->argv->content[0] == '-' && data->argv->content[1] == 'n')
        check_option(&data->argv, &option_n);
    while (data->argv)
    {
        if (option_n && data->argv->content[0] == '-' && data->argv->content[1] == 'n')
            check_option(&data->argv, &option_n);
        if (data->argv->content[0] == '$' && data->argv->content[1])
		{
			exp_env = get_env_var(env, data->argv->content + 1);
			if (exp_env)
				write(1, exp_env->value, ft_strlen(exp_env->value));
		}
        else if (data->argv->content[0] != '#')
            write(1, data->argv->content, ft_strlen(data->argv->content));
        else if (data->argv->content[0] == '#')
            break ;
        data->argv = data->argv->next;
        if (data->argv)
            write(1, " ", 1);
    }
    if (!option_n)
        write(1, "\n", 1);
    *gl_stat = 0;
}

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
    return (0);
}

static void    cd_with_no_args(t_env *env, t_argv *updated_oldpwd, t_argv *updated_pwd, int *status)
{
    t_env   *home;
    char    *path;
    t_chain *data;

    data = ft_malloc_bkol(sizeof(t_chain), BKOLANI);
    // data->adj_f = NULL;
    // data->blk_f = NULL;
    data->next = NULL;
    updated_oldpwd->next = NULL;
    updated_pwd->next = NULL;
    home = get_env_var(env, "HOME");
    path = getcwd(NULL, 0);
    updated_oldpwd->content = ft_strjoin("OLDPWD=", path, BKOLANI);
    data->argv = updated_oldpwd;
    export(env, data);
    if (cd_executor(home->value, path, status))
        return ;
    // if (chdir(home->value) == -1)
    // { 
    //     free(path);
    //     perror("cd");
    //     return ;
    // }
    free(path);
    path = NULL;
    path = getcwd(NULL, 0);
    updated_pwd->content = ft_strjoin("PWD=", path, BKOLANI);
    data->argv = NULL;
    data->argv = updated_pwd;
    export(env, data);
    free(path);
}

static int    cd_with_args(t_env *env, t_argv *argv, t_argv *updated_oldpwd, t_argv *updated_pwd)
{
    char    *path;
    t_chain *data;
	t_env	*exp_env;
    int     status;

    data = ft_malloc_bkol(sizeof(t_chain), ALLOCATE);
    // data->adj_f = NULL;
    // data->blk_f = NULL;
    data->next = NULL;
    updated_oldpwd->next = NULL;
    updated_pwd->next = NULL;
    path = getcwd(NULL, 0);
    updated_oldpwd->content = ft_strjoin("OLDPWD=", path, BKOLANI);
    data->argv = updated_oldpwd;
    export(env, data);
    if (argv->content[0] == '$' && argv->content[1])
	{
		exp_env = get_env_var(env, data->argv->content + 1);
		if (exp_env)
        {
            if (cd_executor(exp_env->value, path, &status))
                return (1);
			// if (chdir(argv->content) == -1)
            // {  
            //     free(path);
            //     perror("cd");
            //     return ;
            // }
        }
	}
    else
    {
        if (cd_executor(argv->content, path, &status))
            return (1);
    }
    // if (chdir(argv->content) == -1)
    // {  
    //     free(path);
    //     perror("cd");
    //     return ;
    // }
    path = NULL;
    path = getcwd(NULL, 0);
    updated_pwd->content = ft_strjoin("PWD=", path, BKOLANI);
    free(data->argv);
    data->argv = NULL;
    data->argv = updated_pwd;
    export(env, data);
    free(path);
    return (0);
}

void    cd(t_env *env, t_chain *data, int *status)
{
    t_argv *updated_oldpwd;
    t_argv *updated_pwd;

    updated_oldpwd = ft_malloc_bkol(sizeof(t_argv), ALLOCATE);
    updated_pwd = ft_malloc_bkol(sizeof(t_argv), ALLOCATE);
    if (data->argv == NULL)
        cd_with_no_args(env, updated_oldpwd, updated_pwd, status);
    else
    {
        if (data->argv->next)
        {
            write(2, "cd: too many arguments\n", 23);
            *status = 1;
            return ;
        }
        *status = cd_with_args(env, data->argv, updated_oldpwd, updated_pwd);
    }
}

// pwd
void    pwd(t_chain *data)
{
    char *path;

    if (!data)
        return ;
    // create_files_only(data);
    // if (data->argv)
    // {
    //     write(2, "pwd: too many arguments\n", 24);
    //     exit(1);
    // }
    path = getcwd(NULL, 0);
    if (!path)
    {
        perror("pwd");
        return ;
    }
    printf("%s\n", path);
    free(path);
}
