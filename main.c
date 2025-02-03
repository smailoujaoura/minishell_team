#include "minishell.h"

void	handle_interrupt(int signum)
{
	(void)signum;
	write(2, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_signals(void)
{
	signal(SIGINT, handle_interrupt);
}

void	exit_shell(void)
{
	exit(0);
}

char	*get_line(void)
{
	char    *line;

	line = readline("Minishell:$ ");
	if (line)
		add_history(line);
	return (line);
}

int	peek(char *s, char *tokens)
{
	char	*tok;

	tok = tokens;
	while (*tok)
	{
		if (*s == *tok)
			return (1);
		tok++;
	}
	if (*s == '&' && s + 1 && *(s + 1) == '&')
		return (1);
	return (0);
}

void	handle_words(char **start)
{
	if (**start == '\0' ||
		ft_strchr(WHITESPACE, **start) || peek(*start, "|<>)"))
		return ;
	else if (**start == '"')
	{
		(*start)++;
		while (**start != '"')
			(*start)++;
		(*start)++;
	}
	else if (**start == '\'')
	{
		(*start)++;
		while (**start != '\'')
			(*start)++;
		(*start)++;
	}
	else
	{
		while (!ft_strchr(SYMBOLS, **start) && !(ft_strchr(WHITESPACE, **start))
			&& !(**start == '&' && *(*start + 1) == '&'))
				(*start)++;
	}
	handle_words(start);
}

int	get_token(char **start, char **tok_start, char **tok_end)
{
	while (**start && ft_strchr(WHITESPACE, **start))
		(*start)++;
	if (tok_start)
		*tok_start = *start;
	if (**start == '\0')
		return (0);
	else if (**start == '(' || **start == ')')
		(*start)++;
	else if (**start == '<')
	{
		(*start)++;
		if (**start == '<')
			(*start)++;
	}
	else if (**start == '>')
	{
		(*start)++;
		if (**start == '>')
			(*start)++;
	}
	else if (**start == '|')
	{
		(*start)++;
		if (**start == '|')
			(*start)++;
	}
	else if (**start == '&' && *(*start + 1) == '&')
		(*start) += 2;
	else
		handle_words(start);
	// while (**start && !ft_strchr(WHITESPACE, **start) && !ft_strchr(SYMBOLS, **start))
	// 	(*start)++; why would we skip the WHITESPACES TWICE, at the beginning is ennough
	if (tok_end)
		*tok_end = *start;
	return (1);
}

t_chain	*convert_str(char *str)
{
	char	*tok_start;
	char	*tok_end;
	t_chain	*list;

	list = NULL;
	while (get_token(&str, &tok_start, &tok_end))
		lstadd_back(&list, lstnew(ft_substr(tok_start, 0, tok_end - tok_start)));
	return (list);
}

t_chain	*check_syntax(t_chain *list)
{
	(void)list;
	return (0);
}

void	find_type(t_chain *list)
{
	if (list->content[0] == '*')
		list->type = WILDCARD;
	else if (ft_strchr(list->content, '"') || ft_strchr(list->content, '\'')) // maybe just check the biggning instead of seaching the whole string
	{
		list->type = WORD;
		if (ft_strchr(list->content, '$') && ft_strchr(list->content, '"'))
			list->dollar = DOLLAR;
	}
	else if (list->content[0] == '$')
		list->type = DOLLAR;
	else
		list->type = WORD;
}

void	tokenize_list(t_chain *list)
{
	while(list)
	{
		if (list->content[0] == '(')
			list->type = L_PAREN;
		else if (list->content[0] == ')')
			list->type = R_PAREN;
		else if (list->content[0] == '|' && list->content[1] == '|')
			list->type = OR;
		else if (list->content[0] == '|')
			list->type = PIPE;
		else if (list->content[0] == '&' && list->content[1] == '&')
			list->type = AND;
		else if (list->content[0] == '>' && list->content[1] == '>')
			list->type = REDIR_APPEND;
		else if (list->content[0] == '>')
			list->type = REDIR_OUT;
		else if (list->content[0] == '<' && list->content[1] == '<')
			list->type = HEREDOC;
		else if (list->content[0] == '<')
			list->type = REDIR_IN;
		else
			find_type(list);
		list = list->next;
	}
}

void	prioritize_list(t_chain *list)
{
	while (list)
	{
		if (list->type == L_PAREN || list->type == R_PAREN)
			list->lvl = NAN;
		else if (list->type == WORD || list->type == REDIR_APPEND || list->type
			== REDIR_IN || list->type == REDIR_OUT || list->type == HEREDOC) // HEREDOC REDIRS
			list->lvl = VIP;
		else if (list->type == PIPE)
			list->lvl = LVL1;
		else if (list->type == OR || list->type == AND)
			list->lvl = LVL2;
		list = list->next;
	}
}

void	assign_depth(t_chain *list)
{
	int	depth;

	depth = 0;
	while (list)
	{
		if (list->type == L_PAREN)
		{
			depth++;
			list->depth = depth;
		}
		else if (list->type == R_PAREN)
		{
			list->depth = depth;
			depth--;
		}
		else
			list->depth = depth;
		list = list->next;
	}
}

t_chain	*convert_infix(t_chain *infix)
{
	t_chain	*post;
	t_chain	*ops;

	post = NULL;
	ops = NULL;
	while (infix)
	{
		if (infix->lvl == VIP)
			move_item(&infix, &post, 0); 
		else
		{
			if ((!ops || infix->lvl == NAN) && infix->type != R_PAREN)
				move_item(&infix, &ops, 1);
			else if (ops && ops->lvl && infix->lvl >= ops->lvl)
			{
				while (ops && ops->lvl && ops->lvl >= infix->lvl)
					move_item(&ops, &post, 0);
				move_item(&infix, &ops, 1);
			}
			else if (infix->type == R_PAREN)
			{
				while (ops && ops->type != L_PAREN)
					move_item(&ops, &post, 0);
				delete_one(&infix, 1);
				delete_one(&ops, 1);
			}
			else if (ops->type == L_PAREN)
				move_item(&infix, &ops, 1);
			else
				printf("handle this shit ls unclosed parenthesis *\n");
		}
	}
	while (ops)
		move_item(&ops, &post, 0);
	return (post);
}


char	*copy_if(char *str, char *s, char *f)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (s[i] == 0)
		{
			f[k] = str[i];
			k++;
		}
		i++;
	}
	f[k] = '\0';
	free(str);
	free(s);
	return (f);
}

int	count_removables(char *s)
{
	int i;
	int	rem;

	i = 0;
	rem = 0;
	while (s[i])
	{
		if (s[i] == 1)
			rem++;
		i++;
	}
	return (rem);
}

char	*remove_occurences(char *str, int i, int singles, int doubles)
{
	char	*s;
	char	*f;

	s = ft_calloc(ft_strlen(str) + 1, 1);
	while (str[i])
	{
		if (str[i] == '"' && singles != 1)
			doubles++;
		if (str[i] == 39 && doubles != 1)
			singles++;
		if (doubles == 2)
			doubles = 0;
		if (singles == 2)
			singles = 0;
		if ((singles != 1 && doubles != 2 && str[i] == '"')
			|| (doubles != 1 && singles != 2 && str[i] == 39))
			s[i] = 1;
		i++;
	}
	f = malloc(ft_strlen(str) - count_removables(s) + 1);
	return (copy_if(str, s, f));
}

void	strip_words(t_chain *list)
{
	while (list)
	{
		if (list->type == WORD)
			list->content = remove_occurences(list->content, 0, 0, 0);
		list = list->next;
	}
}

void	delete_any(t_chain *ptr, int i)
{
	t_chain	*prev;

	prev = NULL;
	if (ptr->back != NULL)
		prev = ptr->back;
	prev->next = ptr->next;
	if (ptr->next)
		ptr->next->back = prev;
	if (i)
		free(ptr->content);
	free(ptr);
}

t_chain	*join_redirs(t_chain *list)
{
	while (list)
	{
		if (list->type == REDIR_IN || list->type == REDIR_OUT || list->type == REDIR_APPEND)
		{
			list->file = list->next->content;
			delete_any(list->next, 0);
			// handle ambiguous syntax when given * if it expands to more than it should.
		}
		if (list->type == HEREDOC)
		{
			list->delim = list->next->content;
			delete_any(list->next, 0);
		}
		list = list->next;
	}
	return (list);
}

void	remove_if(t_chain *list)
{
	t_chain	*tmp;

	while(list)
	{
		tmp = list;
		list = list->next;
		if (tmp->type == REMOVE)
			delete_any(tmp, 0);
	}
}

t_chain	*join_commands(t_chain *list)
{
	t_argv	*argv;
	t_argv	*new;
	t_chain	*ptr;

	argv = NULL;
	while (list)
	{
		if (list->type == WORD)
		{
			ptr = list->next;
			while (ptr && (ptr->type == WORD || ptr->type == WILDCARD || is_redir(ptr, IN + OR + OUT) ))
			{
				if (ptr->type == WORD || ptr->type == WILDCARD)
				{
					new = lstnew_arg(ptr->content);
					new->type = ptr->type;
					lstadd_back_arg(&argv, new);
					ptr->type = REMOVE;
				}
				ptr = ptr->next;
			}
			remove_if(list);
		}
		list->argv = argv;
		list = list->next;
	}
	return (list);
}

void print_list(t_chain *list, char *s)
{
	printf("%s:\n", s);
	t_chain *ptr = list;
	printf("INFIX NOTATION\n");
	while (ptr != NULL)
	{
		printf("[%s]\t\t[%d]\t\t[%d]\t\t[%d]\n", ptr->content, ptr->depth, ptr->lvl, ptr->type);
		ptr = ptr->next;
	}
}

int	is_redir(t_chain *ptr, int f)
{
	if (!ptr)
		return (0);
	if (f == IN + OR + OUT && (is_redir(ptr, IN) || is_redir(ptr, OUT)))
		return (1);
	if (f == IN && (ptr->type == HEREDOC || ptr->type == REDIR_IN))
		return (1);
	else if (f == OUT && (ptr->type == REDIR_APPEND || ptr->type == REDIR_OUT))
		return (1);
	return (0);
}

void	remove_adjacent_redirs(t_chain *list, t_chain *redirs, int f)
{
	t_chain	*non_redir;
	t_chain	*tmp;

	non_redir = redirs;
	while (non_redir && is_redir(non_redir, IN + OR + OUT))
	{
		if (f)
			non_redir = non_redir->back;
		else
			non_redir = non_redir->next;
	}
	if (f)
	{
		if (non_redir)
			non_redir->next = list;
		list->back = non_redir;
	}
	else
	{
		list->next = non_redir;
		if (non_redir)
			non_redir->back = list; // if non_redir is valid 
	}
	while (is_redir(redirs, IN + OR + OUT))
	{
		tmp = redirs->next;
		free(redirs); // need to free content as well.
		redirs = tmp;
	}
}

void	assign_adjacent_redirs(t_chain *list, t_chain *ptr)
{
	t_chain	*last;

	while (list)
	{
		if (list->type == WORD && is_redir(list->back, IN + OR + OUT))
		{
			ptr = list->back;
			while (ptr && is_redir(ptr, IN + OR + OUT))
				ptr = ptr->back;
			list->adj_f = create_redirs_chain(ptr->next);
			remove_adjacent_redirs(list, ptr->next, 1);
		}
		if (list->type == WORD && is_redir(list->next, IN + OR + OUT))
		{
			ptr = list->next;
			if (list->adj_f)
			{
				last = lstlast(list->adj_f);
				last->next = create_redirs_chain(ptr);
				last->next->back = last;
			}
			else
				list->adj_f = create_redirs_chain(ptr);
			remove_adjacent_redirs(list, ptr, 0);
		}
		list = list->next;
	}
}

t_chain	*create_redirs_chain(t_chain *list)
{
	t_chain	*redirs;
	t_chain	*new;

	redirs = NULL;
	while (list && is_redir(list, IN + OR + OUT))
	{
		new = ft_calloc(1, sizeof(t_chain));
		ft_memcpy(new, list, sizeof(t_chain));
		new->next = NULL;
		new->back = NULL;
		lstadd_back(&redirs, new);
		list = list->next;
	}
	return (redirs);
}

void	add_files(t_chain *list, t_chain *new)
{
	t_chain	*ptr;
	t_chain	*last;

	ptr = list->blk_f;
	if (ptr && ptr->blk_f)
	{
		last = lstlast(ptr->blk_f);
		last->next = new;
		new->back = last;
	}
	else
		list->blk_f = new;
}

void	create_all_redirs(t_chain *list, t_chain *ptr, int f)
{
	t_chain	*redirs_lvl_one;
	t_chain	*last;

	redirs_lvl_one = create_redirs_chain(ptr);
	while (is_redir(ptr, IN + OR + OUT))
		ptr = ptr->next;
	while (ptr)
	{
		if (ptr->back->type == R_PAREN && is_redir(ptr, IN + OR + OUT) && f - 1 > ptr->depth)
		{
			if (redirs_lvl_one)
			{
				last = lstlast(redirs_lvl_one);
				last->next = create_redirs_chain(ptr);
				last->next->back = last;
			}
			else
				redirs_lvl_one = create_redirs_chain(ptr);
		}
		ptr = ptr->next;
	}
	list->blk_f = redirs_lvl_one;
}

void	assign_block_redirs(t_chain *list)
{
	t_chain	*ptr;
	int		f;

	f = -1;
	while (list)
	{
		if (list->type == WORD && list->depth > 0)
		{
			ptr = list;
			f = ptr->depth;
			while (ptr && !(ptr->type == R_PAREN && ptr->depth == f)) // maybe ptr->next->depth
				ptr = ptr->next;
			if (is_redir(ptr->next, IN + OR + OUT))
			{
				if (f > 1)
					create_all_redirs(list, ptr->next, f);
				else
					add_files(list, create_redirs_chain(ptr->next));
			}
		}
		list = list->next;
	}
}

t_chain	*assign_inputs(t_chain *list, t_chain *ptr)
{
	list = assign_inputs_edges(list);
	assign_adjacent_redirs(list, ptr);
	assign_block_redirs(list);
	return (list);
}

void	severe_redirs(t_chain *list)
{
	t_chain	*ptr;

	while (list)
	{
		ptr = list;
		while (ptr && (is_redir(ptr, IN + OR + OUT)))
			ptr = ptr->next;
		if (ptr != list)
		{
			list = list->back;
			list->next = ptr;
			if (ptr)
			{
				ptr->back->next = NULL;
				ptr->back = list;
			}
		}
		list = list->next;
	}
}

void	print_with_files(t_chain *ptr)
{
	while (ptr)
	{
		printf("CONTENT:[%s]\t\t\t\t[%d]\n", ptr->content, ptr->depth);
		if (ptr->type == WORD)
		{
			while (ptr->adj_f)
			{
				printf("\t[%s]\t[%s]\n", ptr->adj_f->file, ptr->adj_f->delim);
				ptr->adj_f = ptr->adj_f->next;
			}
			while (ptr->blk_f)
			{
				printf("\t{%s}\t{%s}\n", ptr->blk_f->file, ptr->blk_f->delim);
				ptr->blk_f = ptr->blk_f->next;
			}
			t_argv *args = ptr->argv;
			while (args)
			{
				printf("\targ:[%s]\n", args->content);
				args = args->next;
			}
		}
		ptr = ptr->next;
	}
}

t_chain	*assign_inputs_edges(t_chain *list)
{
	t_chain	*redirs;
	t_chain	*tmp;

	if (is_redir(list, IN + OR + OUT))
	{
		tmp = list;
		redirs = create_redirs_chain(list);
		while (list && is_redir(list, IN + OR + OUT))
			list = list->next;
		if (list && list->type == WORD) // has to be word or else might be syntax error
		{
			list->adj_f = redirs; // bastard redirections
			remove_adjacent_redirs(list, tmp, 1);
		}
		else
			printf("handle this very special case\n"); // there is nothing else just a bunch of redirs; bastard redirs are not adopted by any operator or command
	}
	return (list);
}

// TO DO:
// - Today:
// 		- handle orphan redirections by creating an empty word adjacent to them that executes nothing
// 		- handle wildcard * 
// 		- start execution of the tree
// Tomorrow:
// 	- The beast: Syntax errors handling; unexpected tokens etc. 
t_ast	*parse_line(char *line)
{
	t_chain	*list;
	t_chain	*post;
	t_ast	*root;

	list = convert_str(line); // convert line into linked list of seperated symbols
	tokenize_list(list); // assign types to those symbols 
	prioritize_list(list); // give priority to | over && ||, NAN to (), VIP to words or COMMANDS, priority does not matter for other things redirs etc... this function can go just above convert to postfix
	assign_depth(list); // assign depth of each block, commands inside NO () get depth 0, inside one pair of () get 1, inside (()()) pairs get 2
	strip_words(list); // remove single/double quotes where applicable
	join_redirs(list); // join redirections as in convert_str hands us back ">, file1" from ">file1", as we join the two nodes into ">file1", 
	join_commands(list); // pick up all args of a command into linked list t_argv

	list = assign_inputs(list, NULL); // assign inputs of each command, edges cases, adjacent, block as well.
	severe_redirs(list); // this should more or less delete the severe and free redirections THIS MIGHT NOT BE NEEDED
	post = convert_infix(list); // remove the redirections so that algo for post won't have more types than expected
	print_with_files(post);
	root = build_tree(post);
	// collect_garbage(post);
	return (root);
}

void	loop_minishell(void)
{
	char	*line;
	t_ast	*root;
	
	while (1337)
	{
		line = get_line();
		if (line == NULL)
			exit_shell();
		root = parse_line(line);
		// free line
		// execute tree
		// free tree and other stuff
		(void)root;
	}
}


int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	t_env *env;

	// initalize structures and envirnoment
	env = handle_env(envp);
	// export_env_var(env, "NAME2=kol");
	// export_env_var(env, "NAME2=ani");
	// export_env_var(env, "NAME5=kol");
	// export_env_var(env, "NAME6=kol");
	// export_env_var(env, "NAME7=kol");
	// export_env_var(env, "NAME3=ani");
	// print_env_vars(env);
	// unset_env_var(env, "NAME5");
	// print_env_vars(env);
	// while (env)
	// {
	// 	free(env->full);
	// 	free(env->key);
	// 	free(env->value);
	// 	free(env);
	// 	env = env->next;
	// }
	// export_with_no_args(env);
	// handle signals:
	// handle_signals();
	// cd(env, NULL);
	// cd(env, "..");
	// pwd();
	// cd(env, "mfjdjhd");
	// pwd();
	// print_env_vars(env);
	// echo("echo -nnnnn -nnnnnnnn", "Hello");
	loop_minishell();
}