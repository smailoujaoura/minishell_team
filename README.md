Don't ever hard code! 
Always do good reasearch before starting a project or you will end up hard-coding and regretting it. 
Good research should lead to an immaculate all-encapsulating code, making the project spot on. 

Almost the first thought-of path for solution will not be the best one, breaking all the rules above.

<!-- - fix file descriptor leaks when doing << 1 << 2 << 3 ||| 
	- might require changing the approach of handling signals in readline in here-document by a process like -->

<!-- - handle env -i ./minishell -->


<!-- - fix issues mkdir -p 1/2/3/4   bash shows errors only 3, we show that 4 times  -->


- Makefile relink and tests etc...

- 100 advanced tests for parsing 
- 100 advanced tests for expanding

- echo "$$"


________________________________________________________TO-DOs_________________________________
Minishell: $l $P $f $h
[1]    1898990 segmentation fault (core dumped)  ./minishell
--------------------------------------------------------------
Minishell: export x=" "
Minishell: $x
[1]    1900092 segmentation fault (core dumped)  ./minishell
--------------------------------------------------------------
Minishell: export x="   "
Minishell: $x
        : command not found
--------------------------------------------------------------
file leak : Minishell: echo ""

Minishell: echo ""

Minishell: echo ""

Minishell: echo ""

Minishell: echo ""

Minishell: echo ""

Minishell: echo ""

Minishell: 
--------------------------------------------------------------
Minishell: ls | exit 100
minishell: exit: too many arguments
--------------------------------------------------------------
Minishell: cat >> << ls
Minishell: syntax error near unexpected token `<<'
> ss
> s
> s
> s
> s
> ls
Minishell: echo $?
0
--------------------------------------------------------------
Minishell: ls >>
Minishell: syntax error near unexpected token `newline'
Minishell: echo $?
0
--------------------------------------------------------------
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: ^C
Minishell: echo $?
0
--------------------------------------------------------------
Minishell: ls''''
ls': command not found
Minishell: ls""
ls": command not found-----------------------------------------------------------
Minishell: echo $?
0
--------------------------------------------------------------
Minishell: export x="ls -l^C
Minishell: echo $x
ls -la
Minishell: export y=$x
Minishell: echo $y
ls
--------------------------------------------------------------
inishell: export $x="yassine"^C
Minishell: echo $x
a b
Minishell: export $x="yassine"
Minishell: 
--------------------------------------------------------------
Minishell: echo $EMPTY="yassine"
=yassine
Minishell: export ="ss"
Minishell: 
--------------------------------------------------------------
Minishell: $l | kgdkj
kgdkj: command not found
Minishell: echo $?
1
--------------------------------------------------------------
Minishell: export x="+=+=+=+="
Minishell: echo $x
+
--------------------------------------------------------------
Minishell: export $X=$U yassine=ddd
[1]    1938868 segmentation fault (core dumped)  ./minishell
-----------------------------------------------------------
Minishell: syntax error near unexpected token `>>'
> dd
> d
> d
> d
> l
> <<
> l
> l
> ^C
-----------------------------------------------------------
Minishell: mkdir -p 1/2/3/4/5/6/7
Minishell: ls
"'"   1        builtins   expander   folder     main.c     minishell     parser   TODO
'*'   1.tldr   executor   file       includes   Makefile   minishell.c   t.c      utils
Minishell: cd 1/2/3/4/5/6/7
Minishell: ls
Minishell: pwd
/home/soujaour/Desktop/youbrhic/1/2/3/4/5/6/7
Minishell: pwd
/home/soujaour/Desktop/youbrhic/1/2/3/4/5/6/7
Minishell: unset PATH PWD OLDPWD HOME
Minishell: cd ll
cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
Minishell: cd ..
cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
Minishell: cd
[1]    1945364 segmentation fault (core dumped)  ./minishell
-------------------------------------------------------------
Minishell: echo $x
*
Minishell: cat $x
*
1.tldr
builtins
executor
expander
folder
includes
main.c
Makefile
minishell
minishell.c
parser
t.c
TODO
utils
-------------------------------------------------------------
Minishell: unset HOME
Minishell: cd
[1]    1964739 segmentation fault (core dumped)  ./minishell
-------------------------------------------------------------
Minishell: export cmd1="ls -la" cmd2="cat -e" cmd3="1"
Minishell: export a="c" b="a" c="t"
[1]    1964823 segmentation fault (core dumped)  ./minishell
-------------------------------------------------------------
Minishell: < TODO  cat
[1]    1967587 segmentation fault (core dumped)  ./minishell
-------------------------------------------------------------
Minishell: syntax error near unexpected token `<<'
> file
> a
> b
> c
> <<
> >>
> 
minishell: warning: here-document at line 20 delimited by end-of-file (wanted `|')
Minishell: echo $?
0


--------------------
fix export function 
- fix everything 

Minishell: export cmd1="ls -la" cmd2="cat -e" cmd3="1"
Minishell: export a="c" b="a" c="t"

unset HOME or anything else 
cd 

export var"'"=hsjsj
bash: export: `var'=hsjsj': not a valid identifier
---------------------
bash: ./parser/: Is a directory
./parser
minishell: ./parser: Permission denied
------------------
bash: echo      \n hola
n hola
echo      \n hola
n hola
-----------------

---------------
------------------------------------------------------------------------------------------------------------------------
Minishell: echo "" "" "" "" $HOME
    " " /home/souj
------------------------------------------------------------------------------------------------------------------------
Minishell: echo '' $HOME
  /home/soujaou
------------------------------------------------------------------------------------------------------------------------
Minishell: echo ""             hola
  hol
------------------------------------------------------------------------------------------------------------------------
Minishell: echo "" testing what is what
  testin  wha  i  wha
------------------------------------------------------------------------------------------------------------------------
$
------------------------------------------------------------------------------------------------------------------------
memmory leaks: should not keep incrasing without ever decreasing 
------------------------------------------------------------------------------------------------------------------------
Minishell: ls | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc | wc
      1       3      24
Zombies
------------------------------------------------------------------------------------------------------------------------
export ""
------------------------------------------------------------------------------------------------------------------------
export =
------------------------------------------------------------------------------------------------------------------------
<a cat <b <c 
parsing problem SEGV 
------------------------------------------------------------------------------------------------------------------------
Minishell: ls | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc 
      1       3      24
Minishell: ls | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc  | wc 
      1       3      24
Minishell: ls | wc

=================================================================
==1232472==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x49a29d in malloc (/home/soujaour/Documents/shell/minishell+0x49a29d)
    #1 0x7f6537137bac in xmalloc (/lib/x86_64-linux-gnu/libreadline.so.8+0x39bac)

Indirect leak of 204 byte(s) in 12 object(s) allocated from:
    #0 0x49a29d in malloc (/home/soujaour/Documents/shell/minishell+0x49a29d)
    #1 0x7f6537137bac in xmalloc (/lib/x86_64-linux-gnu/libreadline.so.8+0x39bac)

SUMMARY: AddressSanitizer: 236 byte(s) leaked in 13 allocation(s).
     14      14     134

=================================================================
==1232473==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 32 byte(s) in 1 object(s) allocated from:
    #0 0x49a29d in malloc (/home/soujaour/Documents/shell/minishell+0x49a29d)
    #1 0x7f6537137bac in xmalloc (/lib/x86_64-linux-gnu/libreadline.so.8+0x39bac)

Indirect leak of 204 byte(s) in 12 object(s) allocated from:
    #0 0x49a29d in malloc (/home/soujaour/Documents/shell/minishell+0x49a29d)
    #1 0x7f6537137bac in xmalloc (/lib/x86_64-linux-gnu/libreadline.so.8+0x39bac)

SUMMARY: AddressSanitizer: 236 byte(s) leaked in 13 allocation(s).
Minishell:
------------------------------------------------------------------------------------------------------------------------
echo """"
------------------------------------------------------------------------------------------------------------------------
export var="ls       -la"
------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
-------------
---------------
bash: $?$?
127127: command not found
Minishell: $?$?
11: command not found
--------------
Minishell: ?$HOME
minishell: No such file or directory
bash:  ?$HOME
bash: ?/home/bkolani: No such file or directory
-------------------------------------------------------------------------------------------------------------------------
export var+=
-------------------------------------------------------------------------------------------------------------------------
export var="one = two"
-------------------------------------------------------------------------------------------------------------------------

export var+=test  UPDATE
export var=test   CREATE

export "'var=value'"
       0111111111110