#include "../minishell.h"

// void    echo()
// {
//     int i;
//     int j;
//     int k;
//     int option;
//     char *new_str;
//     int   new_len;
//     char    **splited_cmd;

//     new_len = 0;
//     new_str = NULL;
//     option = 0;
//     splited_cmd = ft_split(cmd, ' ');
//     i = 0;
//     while (splited_cmd[i])
//         i++;
//     k = 1;
//     while (k < i)
//     {
//         if (splited_cmd[k] && splited_cmd[k][0] == '-' 
//             && splited_cmd[k][1] == 'n')
//         {
//             j = 2;
//             printf("%c\n", splited_cmd[k][0]);
//             while (splited_cmd[k][j])
//             {
//                 if (splited_cmd[k][j] != 'n')
//                 {
//                     option = 0;
//                     write(1, splited_cmd[k], ft_strlen(splited_cmd[k]));
//                     break ;
//                 }
//                 j++;
//             }
//             // if (splited_cmd[k][j])
//             // {
//             //     while (splited_cmd[k][j] == 'n')
//             //     {
//             //         option++;
//             //         j++;
//             //     }
//             // }
//             // else
//             //     write(1, splited_cmd[k], ft_strlen(splited_cmd[k]));
//             // while (splited_cmd[k][j])
//             // {
//             //     if (splited_cmd[k][j] != 'n')
//             //     {
//             //         write(1, splited_cmd[k], ft_strlen(splited_cmd[k]));
//             //         break ;
//             //     }
//             //     else
//             //         option++;
//             //     j++;
//             // }
//         }
//         else
//             write(1, splited_cmd[k], ft_strlen(splited_cmd[k]));
//         k++;
//     }
//     printf("Option flag: %d\n", option);
//     if (!str)
//         str = ft_strdup("");
//     else
//     {
//         // if (str[0] == '-' && str[1] == 'n')
//         // {
//         //     i = 2;
//         //     while (str[i])
//         //     {
//         //         if (str[i] != 'n')
//         //             option++;
//         //         i++;
//         //     }
//         // }
//         i = 0;
//         while (str[i])
//         {
//             new_len++;
//             while (str[i + 1])
//             {
//                 if (str[i + 1] == ' ')
//                 {
//                     i++;
//                     continue ;
//                 }
//                 else
//                     break ;
//             }
//             i++;
//         }
//         i = 0;
//         j = 0;
//         new_str = malloc(new_len + 1);
//         while (str[i])
//         {
//             if (str[i] == ' ')
//             {
//                 new_str[j] = str[i];
//                 while (str[i + 1] == ' ')
//                     i++;
//             }
//             else
//                 new_str[j] = str[i];
//             j++;
//             i++;
//         }
//     }
//     if (option)
//     {    
//         printf("with option\n");
//         if (new_str)
//         {
//             printf("new_str\n");
//             write(1, new_str, new_len);
//             // free(new_str);
//             // return ;
//         }
//         else
//         {
//             write(1, str, ft_strlen(str));
//             // free(new_str);
//             // return ;
//         }
//     }
//     else
//     {
//         printf("without option\n");
//         if (new_str)
//         {
//             printf("new_str\n");
//             write(1, new_str, new_len);
//             write(1, "\n", 1);
//             // free(new_str);
//             // return ;
//         }
//         else
//         {

//             write(1, str, ft_strlen(str));
//             write(1, "\n", 1);
//             // free(new_str);
//             // return ;
//         }
//     }
//     if (new_str)
//         free(new_str);
// }

void    cd(t_env *env, const char *dir)
{
    t_env   *home;
    char    *new_pwd;
    char    *new_oldpwd;
    char    *path;
    
    home = NULL;
    if (dir == NULL)
    {
        home = get_env_var(env, "HOME");
        dir = home->value;
    }
    path = getcwd(NULL, 0);
    new_oldpwd = ft_strjoin("OLDPWD=", path);
    export_env_var(env, new_oldpwd);
    if (chdir(dir) == -1)
    {  
        free(new_oldpwd);
        free(path);
        printf("%s\n", strerror(errno));
        return ;
    }
    free(path);
    path = NULL;
    path = getcwd(NULL, 0);
    new_pwd = ft_strjoin("PWD=", path);
    export_env_var(env, new_pwd);
    free(new_oldpwd);
    free(new_pwd);
    free(path);
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
