/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:20:08 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/24 21:21:53 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_ast	*make_tree_node(t_chain **node, int flag)
{
	t_ast	*tree;
	t_chain	*new;

	tree = ft_calloc(1, sizeof(t_ast), SOUJAOUR);
	new = ft_calloc(1, sizeof(t_chain), SOUJAOUR);
	ft_memcpy(new, *node, sizeof(t_chain));
	tree->type = flag;
	tree->data = new;
	*node = (*node)->next;
	return (tree);
}

t_ast	*construct_tree(t_chain **node)
{
	if (!node || !*node)
		return (NULL);
	else if ((*node)->type == WORD)
		return (make_tree_node(node,  CMD));
	else if ((*node)->type == PIPE)
		return (make_tree_node(node, PIPE));
	else if ((*node)->type == AND)
		return (make_tree_node(node, AND));
	else if ((*node)->type == OR)
		return (make_tree_node(node, OR));
	else if ((*node)->type == R_PAREN)
		return (make_tree_node(node, SUB));
	else
	{
		printf("Why do we have more node types than we expect\n");
		return (NULL);
	}
}

void	fill_tree_recursively(t_ast	*parent, t_chain **node)
{
	if (node == NULL || *node == NULL || parent->type == WORD)
		return ;
	if (parent->type != R_PAREN)
	{
		parent->right = construct_tree(node);
		fill_tree_recursively(parent->right, node);
		parent->left = construct_tree(node);
		fill_tree_recursively(parent->left, node);
	}
	else
	{
		parent->left = construct_tree(node);
		fill_tree_recursively(parent->left, node);
	}
}

t_chain	*reverse_list(t_chain *list)
{
	t_chain	*head;
	t_chain	*new;

	head = NULL;
	while (list)
	{
		new = ft_malloc(sizeof(t_chain), ALLOCATE);
		ft_memcpy(new, list, sizeof(t_chain));
		new->next = NULL;
		new->back = NULL;
		lstadd_back(&head, new);
		list = list->back;
	}
	return (head);
}

void	print_tree_with(t_ast *tree)
{
	if (tree == NULL)
		return ;
	print_tree_with(tree->left);
	print_tree_with(tree->right);
	printf("\n[%s]", tree->data->content);
	t_argv *args = tree->data->argv;
	// printf("\tARGS:\n");
	while (args)
	{
		printf("\t\t[%s]\n", args->content);
		args = args->next;
	}
	t_chain	*redirs = tree->data->adj_f;
	// printf("\tFILES:\n");
	while (redirs)
	{
		if (redirs->type == HEREDOC)
			printf("\t\t[%s]\n", redirs->delim);
		else
			printf("\t\t[%s]\n", redirs->file);
		redirs = redirs->next;
	}
}

t_ast	*build_tree(t_chain *post)
{
	t_ast	*root;
	t_chain	*last;
	t_chain	*list;

	if (!post)
		return (NULL);
	last = lstlast(post);
	list = reverse_list(last);
	root = construct_tree(&list);
	if (list)
		fill_tree_recursively(root, &list);
	return (root);
}