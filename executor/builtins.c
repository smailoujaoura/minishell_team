#include "../minishell.h"

int buildin_redir(t_chain *file)
{
    int fd;

    if (file->type == REDIR_IN)
        fd = open(file->content, O_RDONLY, 0644);
    if (file->type == REDIR_APPEND)
        fd = open(file->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (file->type == REDIR_OUT)
        fd = open(file->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return (fd);
}

// exit
void    mini_exit(t_argv *argv)
{
    int i;
    int status;

    if (!argv || !argv->content)
        exit(0);
    if (argv->next)
    {
        write(2, "exit: too many arguments\n", 25);
        return ;
    }
    i = -1;
    while (argv->content[++i])
    {
        if (argv->content[i] < '0' && argv->content[i] > 9)
        {
            printf("exit: %s: numeric argument required\n", argv->content);
            exit(2);
        }
    }
    status = ft_atoi(argv->content);
    status = status % 256;
    exit(status);
}

// echo
void    check_option(t_argv **argv, int *option_n)
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

void echo(t_argv *argv)
{
    int option_n = 0;

    if (!argv)
    {
        write(1, "\n", 1);
        return;
    }
    if (argv->content[0] == '-' && argv->content[1] == 'n')
        check_option(&argv, &option_n);
    while (argv)
    {
        if (option_n && argv->content[0] == '-' && argv->content[1] == 'n')
            check_option(&argv, &option_n);
        write(1, argv->content, ft_strlen(argv->content));
        argv = argv->next;
        if (argv)
            write(1, " ", 1);
    }
    if (!option_n)
        write(1, "\n", 1);
}

// cd
void    cd_with_no_args(t_env *env, t_argv *updated_oldpwd, t_argv *updated_pwd)
{
    t_env   *home;
    char    *path;

    updated_oldpwd->next = NULL;
    updated_pwd->next = NULL;
    home = get_env_var(env, "HOME");
    path = getcwd(NULL, 0);
    updated_oldpwd->content = ft_strjoin("OLDPWD=", path);
    export_env_var(env, updated_oldpwd);
    if (chdir(home->value) == -1)
    {  
        free(updated_oldpwd->content);
        free(updated_oldpwd);
        free(path);
        printf("%s\n", strerror(errno));
        return ;
    }
    free(path);
    path = NULL;
    path = getcwd(NULL, 0);
    updated_pwd->content = ft_strjoin("PWD=", path);
    export_env_var(env, updated_pwd);
    free(path);
}

void    cd_with_args(t_env *env, t_argv *argv, t_argv *updated_oldpwd, t_argv *updated_pwd)
{
    char    *path;

    updated_oldpwd->next = NULL;
    updated_pwd->next = NULL;
    path = getcwd(NULL, 0);
    updated_oldpwd->content = ft_strjoin("OLDPWD=", path);
    export_env_var(env, updated_oldpwd);
    if (chdir(argv->content) == -1)
    {  
        free(updated_oldpwd->content);
        free(updated_oldpwd);
        free(path);
        printf("%s\n", strerror(errno));
        exit(1) ;
    }
    free(path);
    path = NULL;
    path = getcwd(NULL, 0);
    updated_pwd->content = ft_strjoin("PWD=", path);
    export_env_var(env, updated_pwd);
    free(path);
}

void    cd(t_env *env, t_argv *argv)
{
    t_argv *updated_oldpwd;
    t_argv *updated_pwd;
    t_argv  *temp;

    updated_oldpwd = malloc(sizeof(t_argv));
    updated_pwd = malloc(sizeof(t_argv));
    if (argv == NULL)
        cd_with_no_args(env, updated_oldpwd, updated_pwd);
    else
    {
        temp = argv;
        while (temp)
        {
            if (temp->next)
            {
                write(2, "Minishell: cd: too many arguments\n", 34);
                exit(1);
            }
            temp = temp->next;
        }
        cd_with_args(env, argv, updated_oldpwd, updated_pwd);
    }
    free(updated_oldpwd->content);
    free(updated_pwd->content);
    free(updated_oldpwd);
    free(updated_pwd);
}

// pwd
void    pwd(t_chain *data)
{
    int adj_f;
    // int blk_f;

    if (data->argv)
    {
        write(2, "pwd: too many arguments\n", 24);
        exit(1);
    }
    char *path = getcwd(NULL, 0);
    if (!path)
    {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    else
    {
        while (data->adj_f)
        {
            adj_f = buildin_redir(data->adj_f);
            printf("File: %s\n", data->adj_f->content);
            if (data->adj_f->type == REDIR_IN)
            {
                if (dup2(adj_f, STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            else if (data->adj_f->type == REDIR_OUT 
                || data->adj_f->type == REDIR_APPEND)
                write(adj_f, path, ft_strlen(path));
            close(adj_f);
            data->adj_f = data->adj_f->next;
        }
    }
    printf("Ici\n");
    // printf("%s\n", path);
    free(path);
}
