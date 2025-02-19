/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:10 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/18 17:25:31 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tree(t_ast *root)
{
	if (root == NULL)
		return ;
	print_tree(root->left);
	printf("CONTENT:[%s]\t SIDE:[%d]\n", root->data->content, root->side);
	print_tree(root->right);
}

# define IS_ROOT 1
# define IS_CHILD 0

void	link_parent_child(t_ast *root, t_ast *parent, int type)
{
	if (root == NULL)
		return ;
	if (type)
		root->parent = NULL;
	else
		root->parent = parent;
	link_parent_child(root->left, root, IS_CHILD);
	link_parent_child(root->right, root, IS_CHILD);
}

void	assign_nodes_sides(t_ast *tree, int side)
{
	if (tree == NULL)
		return ;
	tree->side = side;
	assign_nodes_sides(tree->left, LEFT);
	assign_nodes_sides(tree->right, RIGHT);
}



void	print_with_args(t_chain *ptr);

void	*check_heredoc_existence(t_chain *list)
{
	t_chain	*ptr;

	while (list)
	{
		if (list->type == WORD)
		{
			ptr = list->adj_f;
			while (ptr)
			{
				if (ptr->type == HEREDOC)
					here_doc(ptr);
				ptr = ptr->next;
			}
			ptr = list->blk_f;
			while (ptr)
			{
				if (ptr->type == HEREDOC)
					here_doc(ptr);
				ptr = ptr->next;
			}
		}
		if (list->error)
			break ;
		list = list->next;
	}
	return (NULL);
}

t_ast	*parse_line(char *line)
{
	t_chain	*list;
	t_chain	*post;
	t_ast	*root;
	int		error;

	error = 0;
	list = convert_str(line);
	tokenize_list(list);
	error = check_syntax(list, line, 0, 0);
	prioritize_list(list);
	assign_depth(list);
	// strip_words(list);

	join_redirs(list);
	join_commands(list);
	
	list = assign_inputs(list, NULL);
	pick_left_redirs(list);
	// severe_redirs will pick up left redirections and assign them to an EMPTY CMD of type WORD
	// print_with_files(list);
	// print_with_args(list);
	if (error == 1)
		return (check_heredoc_existence(list));

	post = convert_infix(list);
	root = build_tree(post);
	link_parent_child(root, NULL, IS_ROOT);
	assign_nodes_sides(root, ROOT);
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
	line = readline("Minishell: ");
	if (line)
		add_history(line);
	return (line);
	(void)env;
}

void	open_heredocs(t_ast *root, t_chain *cmds, t_chain *redirs)
{
	if (root == NULL)
	{
		printf("hddsh\n");
		return ;
	}
	open_heredocs(root->left, NULL, NULL);
	open_heredocs(root->right, NULL, NULL);
	cmds = root->data;
	while (cmds)
	{
		if (cmds->type == WORD)
		{
			redirs = cmds->adj_f;
			while (redirs)
			{
				if (redirs->type == HEREDOC)
					here_doc(redirs);
				redirs = redirs->next;
			}
			redirs = cmds->blk_f;
			while (redirs)
			{
				if (redirs->type == HEREDOC)
					here_doc(redirs);
				redirs = redirs->next;
			}
		}
		cmds = cmds->next;
	}
}

void	loop_minishell(t_env *env)
{
	char	*line;
	t_ast	*root;
	
	while (1337)
	{
		line = get_line(env);
		if (line == NULL)
			break ;
		root = parse_line(line);
		// if (root)
		// open heredocs
		open_heredocs(root, NULL, NULL);
		// 	// execute tree
		executor(root, env);
		free(line);
		ft_malloc(0, DEALLOCATE);
	}
	ft_malloc_bkol(0, DEALLOCATE);
	// ft_malloc(0, DEALLOCATE);
	// ft_malloc_bkol(0, DEALLOCATE);
}