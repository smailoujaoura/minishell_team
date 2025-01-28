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
			== REDIR_IN || list->type == REDIR_OUT || list->type == HEREDOC) // HEREDOR REDIRS
			list->lvl = VIP;
		else if (list->type == PIPE)
			list->lvl = LVL1;
		else if (list->type == OR || list->type == AND)
			list->lvl = LVL2;
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
			else if (ops && ops->lvl != NAN && infix->lvl >= ops->lvl)
			{
				while (ops && ops->lvl != NAN && infix->lvl >= ops->lvl)
					move_item(&ops, &post, 0);
				move_item(&infix, &ops, 1);
			}
			else if (infix->type == R_PAREN)
			{
				while (ops->type != L_PAREN)
					move_item(&ops, &post, 0);
				delete_one(&infix);
				delete_one(&ops);
			}
			else if (ops->type == L_PAREN)
				move_item(&infix, &ops, 1);
		}
	}
	while (ops)
		move_item(&ops, &post, 0);
	return (post);
}

// basically walk the list recursively constructing the tree
// keep advancing the list and constructing the right node until a node has no right: Word? Redir
// go back in recursion and fill the nodes that have left: pipe, &&, ||, etc. with remaining nodes in list
// Now the qustion is what each node of the tree should be and data they should keep:
/*
	- The type of the current node in the tree: command, pipe, ||, &&, file, etc... 
	- The standard output and input of each command in the tree: 
	- The exit status of command in the node.
	- The command arguments vector and argument number
	- 
*/
t_nodes	*build_tree(t_chain *rrpn)
{
	(void)rrpn;
	return (0);
}

// Still needed:
/*
1) Syntax checking:
	- some tokens cannot be followed by others. && &, &&&, &&|, && ||, && |

2)
	- handle command arguments.
	- Handle heredoc arguments. What should heredoc have?
	- expand the envinroment variables and stuff like that.
	- 
*/
t_nodes	*parse_line(const char *line)
{
	char	*str;
	t_nodes	*root = NULL;
	t_chain	*list;
	t_chain	*postfix;

	str = ft_strdup(line); // NULL check
	list = convert_str(str); // break the string into tokens and create linked list
	tokenize_list(list);
	prioritize_list(list);
	// check_syntax(list); // should be impelemnted or not? Here or in the tree? or when constructing the tree?
	// strip_words(list); strip commands off the quotes if they need to etc.... but not spaces
	// join_commands(list); // this one is most likely needed to be done here
	t_chain *ptr = list;
	printf("INFIX NOTATION\n");
	while (ptr != NULL)
	{
		printf("[%s]\t\t[%d]\t\t[%d]\n", ptr->content, ptr->type, ptr->lvl);
		ptr = ptr->next;
	}
	postfix = convert_infix(list);
	ptr = postfix;
	printf("POSTFIX NOTATION\n");
	while (ptr != NULL)
	{
		printf("[%s]\t\t[%d]\t\t[%d]\n", ptr->content, ptr->type, ptr->lvl);
		ptr = ptr->next;
	}
	printf("REVERSED\n");
	ptr = lstlast(postfix);
	while (ptr)
	{
		printf("[%s]\t\t[%d]\t\t[%d]\n", ptr->content, ptr->type, ptr->lvl);
		ptr = ptr->back;
	}
	root = build_tree(postfix);
	return (root);
}

void	loop_minishell(void)
{
	char	*line;
	t_nodes	*root;
	
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
	(void)envp;
	// initalize structures and envirnoment
	// handle signals:
	// handle_signals();
	loop_minishell();
}