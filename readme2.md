Minishell:$ ls
Minishell:$ pwd
/home/soujaour/Downloads/1/2/3/4/5
Minishell:$ rm -rf ../../../../../1
Minishell:$ pwd
/home/soujaour/Downloads/1/2/3/4/5
Minishell:$ pwd
/home/soujaour/Downloads/1/2/3/4/5
Minishell:$ unset PWD OLDPWD
Minishell:$ pwd
AddressSanitizer:DEADLYSIGNAL
=================================================================
==1123774==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000004 (pc 0x0000004ae6c0 bp 0x7fff7083d7c0 sp 0x7fff7083cf38 T0)
==1123774==The signal is caused by a READ memory access.
==1123774==Hint: address points to the zero page.
    #0 0x4ae6c0 in __sanitizer::internal_strlen(char const*) (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x4ae6c0)
    #1 0x43d086 in printf_common(void*, char const*, __va_list_tag*) (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x43d086)
    #2 0x43e4fe in printf (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x43e4fe)
    #3 0x4d7467 in builtin_pwd (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x4d7467)
    #4 0x4e201f in buildin_excutor (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x4e201f)
    #5 0x4e16a2 in run_cmd (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x4e16a2)
    #6 0x4e0c43 in executor (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x4e0c43)
    #7 0x4cb5e7 in minishell (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x4cb5e7)
    #8 0x4cb9d5 in main (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x4cb9d5)
    #9 0x7fcb5cd68d8f in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #10 0x7fcb5cd68e3f in __libc_start_main csu/../csu/libc-start.c:392:3
    #11 0x41f3e4 in _start (/home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/minishell+0x41f3e4)


=======================
export problems 
============================ RELATED
export var="a b c"
export $var=$var 
========================
cat and kill it from a different palce
=======================
istty set attr etc...
===========================
..
=============================
rl_outstream = stderr;
============================
soujaour@e1r1p3:~$ ..
..: command not found
soujaour@e1r1p3:~$ unset PATH
soujaour@e1r1p3:~$ ..
bash: ..: Is a directory
soujaour@e1r1p3:~$ ..
bash: ..: Is a directory
soujaour@e1r1p3:~$ .
bash: .: filename argument required
.: usage: . filename [arguments]
soujaour@e1r1p3:~$ . ls
bash: ls: No such file or directory
soujaour@e1r1p3:~$ 
=================================
export | wc
bash: wc: No such file or directory
==================================
if there is path and if there is not path how to deal with "parser" etc. is a dir etc.. 
=================================
Minishell:$ cd ..
cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
Minishell:$ pwd
/tmp/1/2/3/4/../../
Minishell:$ cd ..pwd
cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
Minishell:$ cd ..pwd
Minishell:$ pwd
===========================================
cd .
==========================================
.
..
=============================================
export $a=$a
===============================================
cat << 1
$? did not expand
========================================
export one one one one one 
======================================
export order entries in alphabetical order
====================================
cannot unset env 
==========================
run minishell in a non existent working dir
cwd = NULL;
=========
