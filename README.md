Don't ever hard code! 
Always do good reasearch before starting a project or you will end up hard-coding and regretting it. 
Good research should lead to an immaculate all-encapsulating code, making the project spot on. 

Almost the first thought-of path for solution will not be the best one, breaking all the rules above.

- Norminette

- heredoc signals: ^C in heredoc

- // add to exit status when signal some integer 128, wh?

- reexecute in a non leaking fds manner

- segfault: echo .*
	- done
- sort by ascii
	- done


- segfault: "$not_exit" // FIXED BUT STILL 
	- still have this issue: "$NOT"
		- done
- expand $?
	- done
- wildcard expansions: .*
	- done
- ambiguous redirection when $not expands to nothing
	- done
	

[nnsnnnnnnnnnnnnnnnsnnnn]
[ls one twoTESthree four]
ls "$var1"ZZ$var2"$var3"XX "$var4" more
ls one twoZZthree fourXX " more
ls one twoZZthree fourXX " more
ls "$NO"*l
ls "$NO"*lll
ls >f1 >"$f2"*fa