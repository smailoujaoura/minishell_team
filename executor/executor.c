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

// void    buildin_excutor(t_chain *data, t_env *env)
// {
//     if (ft_strncmp("echo", data->content, ft_strlen(data->content)) == 0)
//         echo(data->argv);
//     if (ft_strncmp("cd", data->content, ft_strlen(data->content)) == 0)
//     {
//         if (!data->argv)
//             cd(env, )
//     }
// }
void    create_files(t_chain *data)
{
    while (data->adj_f)
    {
        if (open(data->adj_f->content, O_CREAT, 0644) == -1)
            printf("Frees and exit\n");
        data->adj_f = data->adj_f->next;
    }
}

void    create_pipe(t_ast *tree)
{
    pid_t pid;
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe()");
        exit(1);
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork()");
        exit(1);
    }
    if (pid == 0)
    {
        close(pipe_fd[READ_END]);
        dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
        close(pipe_fd[WRITE_END]);
        // Exec
    }
}

void	executor(t_ast *tree, int *pipe1, int *pipe2)
{
    int pipe_fd[2];

    if (!tree)
        return ;
    if (tree->type == WORD)
    {
        if (tree->data->empty)
            create_files(tree->data);
        else
            printf("Call another function to handle other cases");
        
    }
    else if (tree->type == PIPE)
    {

        left_pipe(tree->left);
        right_pipe(tree->right);
    }
    else if (tree->type == AND)
        run_and();
    else if (tree->type == OR)
        run_or();
    else
        printf("Something went wrong\n");
}