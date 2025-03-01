#include "includes/minishell.h"

int main(void)
{
    pid_t pid;

   pid = fork();
   if (pid == 0)
   {
        printf("I am the child\n");
        sleep(100);
        exit(0);
    }
//     printf("I am the parent\n"); 
    while (1)
        sleep(100);
}