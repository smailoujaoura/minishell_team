/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:39 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 11:58:41 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// // handles the redirs that come before the command node
// void	assign_adjecent_redirs_helper(t_chain *list, t_chain *ptr)
// {
// 	ptr = list->back;
// 	while (ptr->back && is_redir(ptr, IN + OR + OUT))
// 	{
// 		ptr = ptr->back;
// 	}
// 	list->adj_f = create_redirs_chain(ptr);
// 	remove_adjacent_redirs(list, ptr->next, 1);
// }

// // assign redirs for their adjacent nodes: whether they come before or after
// void	assign_adjacent_redirs(t_chain *list, t_chain *ptr)
// {
// 	t_chain	*last;

// 	while (list)
// 	{
// 		if (list->type == WORD && is_redir(list->back, IN + OR + OUT))
// 			assign_adjecent_redirs_helper(list, ptr);
// 		if ((list->type == WORD || list->type == R_PAREN)
// 			&& is_redir(list->next, IN + OR + OUT))
// 		{
// 			ptr = list->next;
// 			if (list->adj_f)
// 			{
// 				last = lstlast(list->adj_f);
// 				last->next = create_redirs_chain(ptr);
// 				last->next->back = last;
// 			}
// 			else
// 				list->adj_f = create_redirs_chain(ptr);
// 			remove_adjacent_redirs(list, ptr, 0);
// 		}
// 		list = list->next;
// 	}
// }

// // create empty CMD that will take the redirs that have no commands
// t_chain	*create_empty_cmd(t_chain *list, t_chain *redirs)
// {
// 	t_chain	*new;

// 	new = NULL;
// 	if (!list)
// 	{
// 		list = lstnew("EMPTY CMD");
// 		list->type = WORD;
// 		list->empty = 1;
// 		list->adj_f = redirs;
// 		return (list);
// 	}
// 	else if (list)
// 	{
// 		new = lstnew("EMPTY CMD");
// 		new->type = WORD;
// 		new->empty = 1;
// 		new->adj_f = redirs;
// 		new->next = list;
// 		new->next->back = new;
// 	}
// 	return (new);
// }

// // assign inputs that have no commands
// t_chain	*assign_inputs_edges(t_chain *list)
// {
// 	t_chain	*redirs;
// 	t_chain	*tmp;

// 	if (is_redir(list, IN + OR + OUT))
// 	{
// 		tmp = list;
// 		redirs = create_redirs_chain(list);
// 		while (list && is_redir(list, IN + OR + OUT))
// 			list = list->next;
// 		if (list && list->type == WORD)
// 		{
// 			list->adj_f = redirs;
// 			remove_adjacent_redirs(list, tmp, 1);
// 		}
// 		else
// 			return (create_empty_cmd(list, redirs));
// 	}
// 	return (list);
// }

// apply Shunting Yard Algorithm on the tokens: words, operators, parenthesis
// resulting in and ouput queue(Reverse Polish Notation or postfix)
t_chain	*convert_infix(t_chain *infix, t_chain *post, t_chain *ops)
{
	while (infix)
	{
		if (infix->lvl == VIP)
			move_item(&infix, &post, 0);
		else if ((!ops || infix->lvl == NAN) && infix->type != R_PAREN)
			move_item(&infix, &ops, 1);
		else if (ops && ops->lvl && infix->type != R_PAREN)
		{
			while (ops && ops->lvl && ops->lvl >= infix->lvl)
				move_item(&ops, &post, 0);
			move_item(&infix, &ops, 1);
		}
		else if (infix->type == R_PAREN)
		{
			while (ops && ops->type != L_PAREN)
				move_item(&ops, &post, 0);
			move_item(&infix, &post, 0);
			delete_one(&ops, 1);
		}
		else if (ops->type == L_PAREN)
			move_item(&infix, &ops, 1);
	}
	while (ops)
		move_item(&ops, &post, 0);
	return (post);
}
