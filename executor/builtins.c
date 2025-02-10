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

void    mini_exit(t_chain *data)
{
    int i;
    int status;

    // create_files_only(data);
    if (!data->argv || !data->argv->content)
        exit(0);
    if (data->argv->next)
    {
        write(2, "exit: too many arguments\n", 25);
        return ;
    }
    i = -1;
    while (data->argv->content[++i])
    {
        if (data->argv->content[i] < '0' && data->argv->content[i] > 9)
        {
            printf("exit: %s: numeric argument required\n", data->argv->content);
            exit(2);
        }
    }
    status = ft_atoi(data->argv->content);
    status = status % 256;
    exit(status);
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

void echo(t_chain *data)
{
    int option_n = 0;

    if (!data->argv)
    {
        // redir_output(data, "\n");
        return;
    }
    if (data->argv->content[0] == '-' && data->argv->content[1] == 'n')
        check_option(&data->argv, &option_n);
    while (data->argv)
    {
        if (option_n && data->argv->content[0] == '-' && data->argv->content[1] == 'n')
            check_option(&data->argv, &option_n);
        // redir_output(data, data->argv->content);
        write(1, data->argv->content, ft_strlen(data->argv->content));
        data->argv = data->argv->next;
        if (data->argv)
            write(1, " ", 1);
            // redir_output(data, " ");
    }
    if (!option_n)
        write(1, "\n", 1);
}

// cd
static void    cd_with_no_args(t_env *env, t_argv *updated_oldpwd, t_argv *updated_pwd)
{
    t_env   *home;
    char    *path;
    t_chain *data;

    // data = malloc(sizeof(t_chain));
    data = ft_malloc_bkol(sizeof(t_chain), BKOLANI);
    data->adj_f = NULL;
    data->blk_f = NULL;
    data->next = NULL;
    updated_oldpwd->next = NULL;
    updated_pwd->next = NULL;
    home = get_env_var(env, "HOME");
    path = getcwd(NULL, 0);
    updated_oldpwd->content = ft_strjoin("OLDPWD=", path, BKOLANI);
    data->argv = updated_oldpwd;
    export(env, data);
    if (chdir(home->value) == -1)
    {  
        // ft_malloc_bkol(0, DEALLOCATE);
        // free(updated_oldpwd->content);
        // free(updated_oldpwd);
        free(path);
        perror("cd");
        exit(EXIT_FAILURE);
    }
    free(path);
    path = NULL;
    path = getcwd(NULL, 0);
    updated_pwd->content = ft_strjoin("PWD=", path, BKOLANI);
    // free(data->argv);
    data->argv = NULL;
    data->argv = updated_pwd;
    export(env, data);
    // free(data);
    free(path);
}

static void    cd_with_args(t_env *env, t_argv *argv, t_argv *updated_oldpwd, t_argv *updated_pwd)
{
    char    *path;
    t_chain *data;

    // data = malloc(sizeof(t_chain));
    data = ft_malloc_bkol(sizeof(t_chain), ALLOCATE);
    data->adj_f = NULL;
    data->blk_f = NULL;
    data->next = NULL;
    updated_oldpwd->next = NULL;
    updated_pwd->next = NULL;
    path = getcwd(NULL, 0);
    updated_oldpwd->content = ft_strjoin("OLDPWD=", path, BKOLANI);
    data->argv = updated_oldpwd;
    export(env, data);
    if (chdir(argv->content) == -1)
    {  
        // free(updated_oldpwd->content);
        // free(updated_oldpwd);
        // ft_malloc_bkol(0, DEALLOCATE);
        free(path);
        perror("cd");
        exit(EXIT_FAILURE) ;
    }
    free(path);
    path = NULL;
    path = getcwd(NULL, 0);
    updated_pwd->content = ft_strjoin("PWD=", path, BKOLANI);
    free(data->argv);
    data->argv = NULL;
    data->argv = updated_pwd;
    export(env, data);
    // ft_malloc_bkol(0, DEALLOCATE);
    // free(data);
    free(path);
}

void    cd(t_env *env, t_chain *data)
{
    t_argv *updated_oldpwd;
    t_argv *updated_pwd;

    // updated_pwd = malloc(sizeof(t_argv));
    // updated_oldpwd = malloc(sizeof(t_argv));
    updated_oldpwd = ft_malloc_bkol(sizeof(t_argv), ALLOCATE);
    updated_pwd = ft_malloc_bkol(sizeof(t_argv), ALLOCATE);
    // create_files_only(data);
    if (data->argv == NULL)
        cd_with_no_args(env, updated_oldpwd, updated_pwd);
    else
    {
        if (data->argv->next)
        {
            write(2, "cd: too many arguments\n", 23);
            exit(EXIT_FAILURE);
        }
        cd_with_args(env, data->argv, updated_oldpwd, updated_pwd);
    }
    // ft_malloc_bkol(0, DEALLOCATE);
    // free(updated_oldpwd->content);
    // free(updated_pwd->content);
    // free(updated_oldpwd);
    // free(updated_pwd);
}

// pwd
void    pwd(t_chain *data)
{
    char *path;

    if (!data)
        return ;
    // create_files_only(data);
    if (data->argv)
    {
        write(2, "pwd: too many arguments\n", 24);
        exit(1);
    }
    path = getcwd(NULL, 0);
    if (!path)
    {
        perror("pwd");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", path);
    // redir_output(data, path);
    free(path);
}
