---------------------------------------------------------------------------------
fix segfault in expanding: export a, > $a
---------------------------------------------------------------------------------
fix command execution flow: PATH=NULL, PATH=".", PATH="/usr/bin", etc.
https://www.tldraw.com/f/e50Yo2cLlUrxL2mAjjEnm?d=v-403.146.3459.2028.page
---------------------------------------------------------------------------------
pipe file descriptors leak 
---------------------------------------------------------------------------------
permission denied has exit status 126 not 1
---------------------------------------------------------------------------------
heredoc file descriptors leak 
Minishell:$ << ee << rr << tt 
wefwe
wefwe

minishell: warning: here-document at line 8 delimited by end-of-file (wanted ee')
> 
minishell: warning: here-document at line 8 delimited by end-of-file (wanted rr')

minishell: warning: here-document at line 8 delimited by end-of-file (wanted `tt')
----
Minishell:$ wefwef | cat << ff 
wefwe
wefwe
ff
wefwef: command not found
wefwe
wefwe
---------------------------------------------------------------------------------
wrong exit status when error in retrieving directory using cd, should not be 0
---------------------------------------------------------------------------------
showing error retriving when "cd ." in a rmeoved directory
---------------------------------------------------------------------------------
memory manangemnet issues in child processes
---------------------------------------------------------------------------------
still reachable memory at program exit
---------------------------------------------------------------------------------
handling system call failures better
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
ls > ""
---------------------------------------------------------------------------------
<< $$$"ls"
---------------------------------------------------------------------------------
wait in sig handler 
---------------------------------------------------------------------------------
learn more about controlling terminal tty and processes
---------------------------------------------------------------------------------
Handle shell level and other dynamic env vars
---------------------------------------------------------------------------------
echo $"USER$User"$PWD