#include "minishell.h"

# define READ_END 0
# define WRITE_END 1

int check_buildin(t_chain *data)
{
    if (ft_strncmp("echo", data->content, ft_strlen(data->content)) == 0
        || ft_strncmp("cd", data->content, ft_strlen(data->content)) == 0
        || ft_strncmp("pwd", data->content, ft_strlen(data->content)) == 0
        || ft_strncmp("export", data->content, ft_strlen(data->content)) == 0
        || ft_strncmp("unset", data->content, ft_strlen(data->content)) == 0
        || ft_strncmp("env", data->content, ft_strlen(data->content)) == 0
        || ft_strncmp("exit", data->content, ft_strlen(data->content)) == 0)
        return (1);
    return (0);
}

void    buildin_excutor(t_chain *data, t_env *env_head)
{
    if (ft_strncmp("echo", data->content, ft_strlen(data->content)) == 0)
        echo(data);
    if (ft_strncmp("cd", data->content, ft_strlen(data->content)) == 0)
        cd(env_head, data);
    if (ft_strncmp("pwd", data->content, ft_strlen(data->content)) == 0)
        pwd(data);
    if (ft_strncmp("export", data->content, ft_strlen(data->content)) == 0)
        export(env_head, data);
    if (ft_strncmp("unset", data->content, ft_strlen(data->content)) == 0)
        unset(env_head, data);
    if (ft_strncmp("env", data->content, ft_strlen(data->content)) == 0)
        mini_env(env_head, data);
    if (ft_strncmp("exit", data->content, ft_strlen(data->content)) == 0)
        mini_exit(data);
}

void    create_files(t_chain *file)
{
  
    if (!file)
        return ;
    while (file)
    {
        if (open(file->file, O_CREAT, 0644) == -1)
            printf("Frees and exit\n");
        file = file->next;
    }
}

int   *create_pipe(void)
{
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe()");
        exit(1);
    }
    return (pipe_fd);
}

// void create_process(t_ast *tree)
// {
//     pid_t pid;

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork()");
//         exit(1);
//     }
//     if (pid == 0)
//     {
//         // close(pipe_fd[READ_END]);
//         // dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
//         // close(pipe_fd[WRITE_END]);
//         // Exec
//     }
// }

void	executor(t_ast *tree, t_env *env)
{
    if (!tree)
        return ;
    if (tree->type == WORD)
    {
        if (tree->data->empty)
        {
            create_files(tree->data->adj_f);
            create_files(tree->data->blk_f);
        }
        else
        {
            
        }
    }
    else if (tree->type == PIPE)
    {

    }
    else if (tree->type == AND)
        run_and();
    else if (tree->type == OR)
        run_or();
    else
        printf("Something went wrong\n");
}
