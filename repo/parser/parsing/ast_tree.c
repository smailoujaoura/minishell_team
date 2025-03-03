/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:20:08 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/03 08:23:36 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// allocate for tree node and assign data to it
t_ast	*make_tree_node(t_chain **node, int type)
{
	t_ast	*tree;

	tree = ft_calloc(1, sizeof(t_ast), SOUJAOUR);
	tree->type = type;
	tree->data = *node;
	tree->f = 0;
	*node = (*node)->next;
	return (tree);
}

// check the type of token and create tree node based on it
t_ast	*construct_tree(t_chain **node)
{
	if (!node || !*node)
		return (NULL);
	else if ((*node)->type == WORD)
		return (make_tree_node(node, CMD));
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

// checks the type of the token in list and creates nodes if necessary:
// subshells have only the left child, right set to NULL
// operators have both the left and right children
// commands have no left or right child, they are set to NULL
void	fill_tree_recursively(t_ast	*parent, t_chain **node)
{
	if (node == NULL || *node == NULL || parent->type == CMD)
		return ;
	if (parent->type != SUB)
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

// reverses list for easy manipulation and understanding
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

// create root node of the tree and then call function to create rest of tree
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
