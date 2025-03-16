---------------------------------------------------------------------------------
fix segfault in expanding: export a, > $a
---------------------------------------------------------------------------------
fix command execution flow: PATH=NULL, PATH=".", PATH="/usr/bin", etc.
https://www.tldraw.com/f/e50Yo2cLlUrxL2mAjjEnm?d=v-403.146.3459.2028.page
---------------------------------------------------------------------------------
pipe file descriptors leak 
---------------------------------------------------------------------------------
&& ls |
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
Minishell:$ k|
> ^C
k: command not found
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
---------------------------------------------------------------------------------














---------------------------------------------------------------------------------
Minishell:$ ls
archive  builtins  executor  expander  includes  main.c  main.o  Makefile  minishell  minishell.c  minishell.o  parser  README.md  utils
Minishell:$ ls | ls
archive   executor  includes  main.o    minishell    minishell.o  README.md
builtins  expander  main.c    Makefile  minishell.c  parser       utils
Minishell:$ ls | $'\t
minishell: syntax error: unquoted string
Minishell:$ ls |
> ls|
> ls |
> |;
Minishell: syntax error near unexpected token ```|'
Minishell:$ |
Minishell: syntax error near unexpected token `|'
Minishell:$ k|
---------------------------------------------------------------------------------
Minishell:$ k|
> ^C
k: command not found
Minishell:$ k |
> ^C
k: command not found
Minishell:$ k| jd
AddressSanitizer:DEADLYSIGNAL

==277040==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x55dbf7533d1d bp 0x7ffccffc7640 sp 0x7ffccffc7630 T0)
==277040==The signal is caused by a READ memory access.
==277040==Hint: address points to the zero page.
    #0 0x55dbf7533d1d in store_line /home/soujaour/Documents/minihell/minishell.c:24
    #1 0x55dbf7533fb3 in complete_line /home/soujaour/Documents/minihell/minishell.c:57
    #2 0x55dbf753435f in parse_line /home/soujaour/Documents/minihell/minishell.c:85
    #3 0x55dbf753455f in minishell /home/soujaour/Documents/minihell/minishell.c:104
    #4 0x55dbf7533b64 in main /home/soujaour/Documents/minihell/main.c:73
    #5 0x7ff0437eed8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #6 0x7ff0437eee3f in __libc_start_main_impl ../csu/libc-start.c:392
    #7 0x55dbf7533724 in _start (/home/soujaour/Documents/minihell/minishell+0x5724)