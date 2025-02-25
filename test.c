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
            execve(argv_1[0], argv_1, envp);
        }
        wait(NULL);  // doing this makes everything break including bad input errors 
        // exit(0);    // Doing this makes everything run smoothly, but 
        // omitting both makes everything hang 
    }
    printf("Bloody hell\n");
    r_pid = fork();
    if (r_pid == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        pid_t two = fork();
        if (two == 0)
        {
            execve(argv_2[0], argv_2, envp);
        }
        wait(NULL);
        // exit(0);
    }
     printf("Bat shit\n");
    close(fd[1]);
    close(fd[0]);
    wait(NULL);
    wait(NULL);
    return 0;
}
