We can inspect the status thanks to several macros:

WIFEXITED(status): returns true if the child terminated normally, for example by calling exit or finishing through the main function of the program.
WEXITSTATUS(status): to be used only if WIFEXITED returned true. Returns the child’s exit code, meaning the number the child specified to the exit function or in the main function’s return.
WIFSIGNALED(status): returns true if the child was forcefully terminated by a signal.
WTERMSIG(status): to be used only if WIFSIGNALED returned true. Returns the signal number that provoked the child’s termination.


--child-silent-after-fork=yes


Open(): O_SYNC: Make file writes synchronous 
		O_ASYNC: Generate a signal when I/O is possible
		O_EXCL: Insure the process is the creator of the file


dup2(old, new) make me a copy of old file descriptor into the supplied new file descriptor which implies anything written into new will go into old; it does close newfd if it is already open; maybe close it explicitly 


The shell opens dir.txt, obtaining a new file descriptor. Then, if that file
descriptor is not standard output, the shell uses dup2() to force standard
output to be a duplicate of the new descriptor and closes the new descriptor,
since it is no longer required. (This method is safer than the preceding
Program Execution
575method, since it doesn’t rely on lower-numbered descriptors being open.)
The code sequence is something like the following:
fd = open("dir.txt", O_WRONLY | O_CREAT,
S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
/* rw-rw-rw- */
if (fd != STDOUT_FILENO) {
dup2(fd, STDOUT_FILENO);
close(fd);
}

The explanation given here of how the shell performs I/O redirections sim-
plifies some points. In particular, certain commands—so-called shell built-in
commands—are executed directly by the shell, without performing a fork() or
an exec(). Such commands must be treated somewhat differently for the purposes
of I/O redirection.

A shell command is implemented as a built-in for either of two reasons:
efficiency and to obtain side effects within the shell.
Other examples of commands that are built in for
their side effects include exec, exit, read, set, source, ulimit, umask, wait, and the
shell job-control commands ( jobs, fg, and bg). The full set of built-in commands
understood by a shell is documented in the shell’s manual page.

The close-on-exec flag (FD_CLOEXEC)
Sometimes, it may be desirable to e

The fcntl() F_GETFD operation
retrieves a copy of the file descriptor flags:


dup2(pfd[1], STDOUT_FILENO);
/* Close descriptor 1, and reopen bound
to write end of pipe */
Pipes and F I FOs
899After duplicating pfd[1], we now have two file descriptors referring to the write end
of the pipe: descriptor 1 and pfd[1]. Since unused pipe file descriptors should be
closed, after the dup2() call, we close the superfluous descriptor:
close(pfd[1]);


if (pfd[1] != STDOUT_FILENO) {
dup2(pfd[1], STDOUT_FILENO);
close(pfd[1]);
}