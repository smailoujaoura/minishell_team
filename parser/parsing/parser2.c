/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:39 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/24 11:07:47 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	delete_any(t_chain *ptr, int i)
{
	t_chain	*prev;

	prev = NULL;
	if (ptr->back != NULL)
		prev = ptr->back;
	prev->next = ptr->next;
	if (ptr->next)
		ptr->next->back = prev;
	// if (i)
	// 	free(ptr->content);
	// free(ptr);
	(void)i;
}

void	remove_if(t_chain *list)
{
	t_chain	*tmp;

	while(list)
	{
		tmp = list;
		list = list->next;
		if (tmp->type == REMOVE || tmp->removable == REMOVE)
			delete_any(tmp, 0);
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
		if ((list->type == WORD || list->type == R_PAREN) && is_redir(list->next, IN + OR + OUT))
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

t_chain	*assign_inputs_edges(t_chain *list)
{
	t_chain	*redirs;
	t_chain	*tmp;
	t_chain	*new;

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
		else if (!list)
		{
			list = lstnew("EMPTY CMD");
			list->type = WORD;
			list->empty = 1;
			list->adj_f = redirs;
		}
		else if (list)
		{
			new = lstnew("EMPTY CMD");
			new->type = WORD;
			new->empty = 1;
			new->adj_f = redirs;
			new->next = list;
			new->next->back = new;
			return (new);
		}
		else
			printf("handle this very special case\n"); // there is nothing else just a bunch of redirs; bastard redirs are not adopted by any operator or command
	}
	return (list);
}

// void	organize_sub(t_chain *r_paren)
// {
// 	r_paren->type = SUBSHELL;
// }

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
			else if (ops && ops->lvl && infix->type != R_PAREN) // should consolidate more these
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
			else
			{
				printf("handle this shit ls unclosed parenthesis *\n"); // it should never ever reach here 
			}
		}
	}
	while (ops)
		move_item(&ops, &post, 0); // there might be some case where we move left parenthesis to the post fix list
	return (post);
}