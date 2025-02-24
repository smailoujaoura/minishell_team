#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int ac, char **av, char **envp)
{
    int fd[2];
    pid_t l_pid;
    pid_t r_pid;
    char buff[100] = {0};
    char *argv_1[2] = {"/bin/ls", NULL};
    char *argv_2[2] = {"/bin/wc", NULL};

    // char *env[1] = {NULL};

    pipe(fd);
    l_pid = fork();
    if (l_pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        pid_t one = fork();
        if (one == 0)
        {
            // printf("%d\n", STDOUT_FILENO);
            execve(argv_1[0], argv_1, envp);
        }
        exit(0);
    }
    r_pid = fork();
    if (r_pid == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        pid_t two = fork();
        if (two == 0)
        {
            // printf("%d\n", STDIN_FILENO);
            execve(argv_2[0], argv_2, envp);
            // read(STDIN_FILENO, buff, 1337);
            // printf("Right received: %s", buff);
        }
        exit(0);
    }
    
    close(fd[0]);
    close(fd[1]);
    return 0;
}
