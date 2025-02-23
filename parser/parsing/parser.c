/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:12:31 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/23 13:36:33 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


// void	severe_redirs(t_chain *list)
// {
// 	t_chain	*ptr;

// 	while (list)
// 	{
// 		ptr = list;
// 		while (ptr && (is_redir(ptr, IN + OR + OUT)))
// 			ptr = ptr->next;
// 		if (ptr != list)
// 		{
// 			list = list->back;
// 			list->next = ptr;
// 			if (ptr)
// 			{
// 				ptr->back->next = NULL;
// 				ptr->back = list;
// 			}
// 		}
// 		list = list->next;
// 	}
// }

void	print_with_files(t_chain *ptr)
{
	while (ptr)
	{
		printf("CONTENT:[%s]\t %d\n", ptr->content, ptr->depth);
		if (ptr->type == WORD)
		{
			t_chain *heredocs = ptr->adj_f;
			while (heredocs)
			{
				printf("[%s]\n", heredocs->delim);
				heredocs = heredocs->next;
			}
			// if (ptr->adj_f)
			// 	printf("%s \t %d\n", ptr->adj_f->file, ptr->adj_f->depth);
			// while (ptr->adj_f)
			// {
			// 	printf("\t[%s]\t[%s]\n", ptr->adj_f->file, ptr->adj_f->delim);
			// 	ptr->adj_f = ptr->adj_f->next;
			// }
			// while (ptr->blk_f)
			// {
			// 	printf("\t{%s}\t{%s}\n", ptr->blk_f->file, ptr->blk_f->delim);
			// 	ptr->blk_f = ptr->blk_f->next;
			// }
			// t_argv *args = ptr->argv;
			// while (args)
			// {
			// 	printf("\targ:[%s]\t\t\tDollar$:[%d]\t\t\tWild*:[%d]\n", args->content, args->dollar, args->wildcard);
			// 	args = args->next;
			// }
		}
		ptr = ptr->next;
	}
}

void	print_with_args(t_chain *ptr)
{
	t_argv	*args;
	t_chain	*files;
	while (ptr)
	{
		printf("CONTENT:[%s]\t depth[%d] wild[%d]  dollar[%d]\n", ptr->content, ptr->depth, ptr->wildcard, ptr->dollar);
		args = ptr->argv;
		while (args)
		{
			printf("\targ[%s] wild[%d] dollar[%d]\n", args->content, args->wildcard, args->dollar);
			args = args->next;
		}
		printf("\n\t\t[FILES]:\n");
		files = ptr->adj_f;
		while (files)
		{
			printf("\t\t\tfile[%s] wild[%d] dollar[%d]\n", files->file, files->wildcard, files->dollar);
			files = files->next;
		}
		ptr = ptr->next;
	}
}

void	pick_left_redirs(t_chain *list)
{
	t_chain	*ptr;
	t_chain	*new;

	while (list)
	{
		ptr = list;
		while (ptr && (is_redir(ptr, IN + OR + OUT)))
			ptr = ptr->next;
		if (ptr != list)
		{
			new = ft_calloc(1, sizeof(t_chain), SOUJAOUR);
			new->type = WORD;
			// GIVE CONTENT "empty for now"
			new->content = "Empty CMD";
			new->empty = 1;
			new->next = ptr;
			new->back = list->back;
			list->back = NULL;
			new->adj_f = list;
			new->back->next = new;
			list = new;
		}
		list = list->next;
	}
}

t_chain	*assign_inputs(t_chain *list, t_chain *ptr)
{
	list = assign_inputs_edges(list);
	assign_adjacent_redirs(list, ptr);
	assign_block_redirs(list);
	remove_if(list);
	return (list);
}
void	join_commands(t_chain *list)
{
	t_argv	*argv;
	t_argv	*new;
	t_chain	*ptr;

	while (list)
	{
		argv = NULL;
		if (list->type == WORD)
		{
			ptr = list->next;
			while (ptr && (ptr->type == WORD || ptr->type == WILDCARD || is_redir(ptr, IN + OR + OUT) ))
			{
				if (ptr->type == WORD || ptr->type == WILDCARD)
				{
					new = lstnew_arg(ptr);
					new->type = ptr->type;
					lstadd_back_arg(&argv, new);
					ptr->type = REMOVE;
				}
				ptr = ptr->next;
			}
			remove_if(list);
			list->argv = argv;
		}
		list = list->next;
	}
}


void	join_redirs(t_chain *list)
{
	while (list)
	{
		if (list->type == REDIR_IN || list->type == REDIR_OUT || list->type == REDIR_APPEND)
		{
			list->file = list->next->content;
			list->wildcard = list->next->wildcard;
			list->dollar = list->next->dollar;
			delete_any(list->next, 0);
			// handle ambiguous syntax when given * if it expands to more than it should.
		}
		if (list->type == HEREDOC)
		{
			list->delim = list->next->content;
			list->delim_in_quotes = list->next->delim_in_quotes;
			delete_any(list->next, 0);
		}
		list = list->next;
	}
}

void	strip_words(t_chain *list)
{
	while (list)
	{
		if (list->type == HEREDOC && list->next && list->next->type == WORD)
		{
			if (ft_strchr(list->next->content, '"') || ft_strchr(list->next->content, '\''))
				list->delim_in_quotes = 1;
		}
		if (list->type == WORD)
			list->content = remove_occurences(list->content, 0, 0, 0);
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
