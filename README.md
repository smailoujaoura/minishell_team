-------------------------------------------------------------------------------- STILL
why do we fork 4 times to execute just one command
--------------------------------------------------------------------------------- STILL NEEDS MORE ATTENTION
- unset PWD "chdir" check the OLDPW
- usnet OLDPWD echo $ should not print when chdir should print
- c^ echo $? not display for first time. ???
- should not inherit OLDPWD AND PWD 
- ulimit -u 3 limit number of forks.
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