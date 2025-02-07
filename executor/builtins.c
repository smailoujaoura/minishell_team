#include "../minishell.h"

void    check_option(t_argv **argv, int *option_n)
{
    int i;

    i = 2;
    while ((*argv)->content[i] && (*argv)->content[i] == 'n')
        i++;
    if ((*argv)->content[i] == '\0')
    {
        option_n++;
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
        return ;
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
                printf("Minishell: cd: too many arguments\n");
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

void    pwd(void)
{
    char *path = getcwd(NULL, 0);
    if (path)
        printf("%s\n", path);
    else
        printf("getcwd failed");
    free(path);
}
