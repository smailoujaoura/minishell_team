#include "minishell.h"

void	print_tree(t_ast *root)
{
	if (root == NULL)
		return ;
	print_tree(root->left);
	printf("[%s]\n", root->data->content);
	print_tree(root->right);
}

t_ast	*parse_line(char *line)
{
	t_chain	*list;
	t_chain	*post;
	t_ast	*root;

	list = convert_str(line);
	tokenize_list(list);

	if (check_syntax(list, line, 0, 0))
		return (free_list(list));
	prioritize_list(list);
	assign_depth(list);
	strip_words(list);

	join_redirs(list);
	join_commands(list);
	list = assign_inputs(list, NULL);
	// severe_redirs will pick up left redirections and assign them to an EMPTY CMD of type WORD
	pick_left_redirs(list);
	print_with_files(list);

	post = convert_infix(list);
	root = build_tree(post);
	printf("\nTree: \n");
	print_tree(root);
	// collect_garbage(post);
	root = NULL;
	return (root);
}

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

char	*get_line(t_env *env)
{
	char    *line;
    t_env    *path;
    t_env   *home;
	char    *new_prompt;

	home = get_env_var(env, "HOME");
	path = get_env_var(env, "PWD");
	new_prompt = ft_strjoin("Minishell:$~", (path->value + ft_strlen(home->value)));
	if (ft_strncmp(path->value, home->value, ft_strlen(path->value)) == 0)
		line = readline("Minishell:$ ");
	else
		line = readline(ft_strjoin(new_prompt, " "));
	if (line)
		add_history(line);
	free(new_prompt);
	return (line);
}

void	loop_minishell(t_env *env)
{
	char	*line;
	t_ast	*root;
	
	while (1337)
	{
		line = get_line(env);
		if (line == NULL)
			exit_shell();
		root = parse_line(line);
		if (root)
			// execute tree
		// free line
		// free tree and other stuff
		(void)root;
	}
}