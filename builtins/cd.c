/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:02 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/28 17:18:20 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int  handle_err(const char *path)
{
    if (!path)
    {
        write(2, "cd: error retrieving current directory: "
            "getcwd: cannot access parent directories: "
            "No such file or directory\n", 108);
        chdir("..");
        return (1);
    }
    return (0);
}

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
    builtin_export(env, updated_oldpwd, 0);
    if (cd_executor(home->value, path, status))
        return ;
    path = getcwd(NULL, 0);
    updated_pwd[0] = ft_strjoin("PWD=", path, BKOLANI);
    updated_pwd[1] = NULL;
    builtin_export(env, updated_pwd, 0);
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
    if (handle_err(path))
        return ;
    updated_oldpwd[0] = ft_strjoin("OLDPWD=", path, BKOLANI);
    updated_oldpwd[1] = NULL;
    builtin_export(env, updated_oldpwd, 0);
    if (cd_executor(argv[1], path, status))
        return ;
    path = getcwd(NULL, 0);
    if (handle_err(path))
        return ;
    updated_pwd[0] = ft_strjoin("PWD=", path, BKOLANI);
    updated_pwd[1] = NULL;
    printf("%s\n", path);
    builtin_export(env, updated_pwd, 0);
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
