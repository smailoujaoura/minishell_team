
--------------------------------------------------------------------------------------------
heredoc file descriptor leaks when you put in ^D
(((((wc << 1) << 2))))
((wc << 1) << 2)
--------------------------------------------------------------------------------------------
soujaour@e2r12p4:~/Documents/project$ echo|
> 
bash: syntax error: unexpected end of file
exit
❯ echo $?
2                    EXIT STATUS IS 2 NOT 1 
--------------------------------------------------------------------------------------------
echo > file < file2
--------------------------------------------------------------------------------------------
echo $(ls)
--------------------------------------------------------------------------------------------
export VAR="value" && echo $VAR) || (echo "error"
--------------------------------------------------------------------------------------------
echo hello |< file99 | >> file2 WRONG EXIT STATUS
--------------------------------------------------------------------------------------------
echo hello | grep pattern > file &&
ls			should not show syntax error
--------------------------------------------------------------------------------------------
FD LEAK: (cat << EOF) | grep pattern
--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------








































--------------------------------------------------------------------------------------------
- run the code through codepilots to deroot any hidden memory issues
- protect everything
- segvs
- compile error in some compilers                                                    ---- DONE
- segv                                                                               ---- DONE
- exit(*status)                                                                      ---- DONE
- do not give the static variabe store_pwd a directory when all options are consumed ---- DONE
- ^CMinishell:$ wc | wc | ls                                                         ---- DONE


TESTS: 
- ( > output.txt)

"
❯ env -i .././minishell
shell-init: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
Minishell:$ env
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ 
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ 
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ 
Minishell:$ pwd
pwd: error tetrieving current directory: getcwd: No such file or directory
Minishell:$ export
Minishell:$ env
Minishell:$ echo $PATH

Minishell:$ export
Minishell:$ export a b
e[1]    320815 segmentation fault (core dumped)  env -i .././minishell
"

" ask | "


-----------------------
env -i bash
shell-init: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
soujaour@e2r12p2:.$ pwd
pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
soujaour@e2r12p2:.$ export
declare -x LC_COLLATE="C"
declare -x OLDPWD
declare -x SHLVL="1"
soujaour@e2r12p2:.$ cd .
chdir: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
soujaour@e2r12p2:.$ pwd
.
soujaour@e2r12p2:.$ cd .
soujaour@e2r12p2:.$ pwd
.
soujaour@e2r12p2:.$ cd .
soujaour@e2r12p2:.$ pwd
.
soujaour@e2r12p2:.$ 
soujaour@e2r12p2:.$ pwd
.
soujaour@e2r12p2:.$ cd ..
soujaour@e2r12p2:..$ pwd
..
soujaour@e2r12p2:..$ 
soujaour@e2r12p2:..$ pwd
..
soujaour@e2r12p2:..$ cd ..
soujaour@e2r12p2:../..$ pwd
../..
soujaour@e2r12p2:../..$ cd ..
soujaour@e2r12p2:../../..$ pwd
../../..
soujaour@e2r12p2:../../..$ 
soujaour@e2r12p2:../../..$ pwd
../../..
soujaour@e2r12p2:../../..$ cd

--------------------------------------------------------------------------------- STILL 
- should not inherit OLDPWD AND PWD 
-------------------------------------------------------------------------------- STILL
- ulimit -u 3 limit number of forks.
-------------------------------------------------------------------------------- STILL
why do we fork 4 times to execute just one command
--------------------------------------------------------------------------------- STILL
- unset PWD "chdir" check the OLDPW
--------------------------------------------------------------------------------- STILL
- unset OLDPWD echo $ should not print when chdir should print
--------------------------------------------------------------------------------- DONE
- c^ echo $? not display for first time. ???
--------------------------------------------------------------------------------- STILL MAYBE NOT REAL
pipe file descriptors leaks when fork fails or something like that
--------------------------------------------------------------------------------- DONE
memory manangemnet issues in child processes
--------------------------------------------------------------------------------- DONE 
still reachable memory at program exit
--------------------------------------------------------------------------------- DONE
fix segfault in expanding: export a, > $a
-------------------------------------------------------------------------------- DONE
fix command execution flow: PATH=NULL, PATH=".", PATH="/usr/bin", etc.
https://www.tldraw.com/f/e50Yo2cLlUrxL2mAjjEnm?d=v-403.146.3459.2028.page
--------------------------------------------------------------------------------- DONE
&& ls |
--------------------------------------------------------------------------------- DONE
permission denied has exit status 126 not 1
--------------------------------------------------------------------------------- DONE
command not found or some other issues
--------------------------------------------------------------------------------- DONE
wrong exit status when error in retrieving directory using cd, should not be 0
--------------------------------------------------------------------------------- DONE
showing error retriving when "cd ." in a rmeoved directory
--------------------------------------------------------------------------------- DONT
handling system call failures better
--------------------------------------------------------------------------------- DONE
Minishell:$ k|, > ^C, k: command not found
--------------------------------------------------------------------------------- DONE
ls > ""
--------------------------------------------------------------------------------- DONT
Handle shell level and other dynamic env vars
--------------------------------------------------------------------------------- DONT
wait in sig handler 
--------------------------------------------------------------------------------- DONE
echo $"USER$User"$PWD
--------------------------------------------------------------------------------- DONT
<< $$$"ls"
--------------------------------------------------------------------------------- STILL
learn more about controlling terminal tty and processes
-------------------------------------------------------------------------------- DONT STILL needs too much hardcode 
- Try this:
 export one="two three"
export two=$one
export three="$one=$two"
export four=$one=$three
compare results of bash and our minishell