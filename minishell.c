/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:10 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/28 22:10:27 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	strip_heredoc(t_chain *node, char *delim)
{
	if (ft_strchr(delim, '"') || ft_strchr(delim, '\''))
		node->delim_in_quotes = 1;
	node->delim = remove_occurences(delim, 0, 0, 0);
}

int	open_heredocs(t_chain *list, int num)
{
	int	execute_or_not;

	execute_or_not = 0;
	while (list)
	{
		if (list->error == 1)
		{
			execute_or_not = 1;
			break ;
		}
		if (list->type == HEREDOC)
		{
			strip_heredoc(list, list->next->content);
			delete_any(list->next, 0);
			here_doc(list, num);
		}
		list = list->next;
	}
	return (execute_or_not);
}

void	store_line(char *new, int flag)
{
	static char	*store;

	if (flag == -1)
	{
		store = NULL;
		return ;
	}
	store = ft_strjoin(store, new, SOUJAOUR);
	if (flag)
	{
		if (store[0])
			add_history(store);
	}
}

int	complete_line(t_chain *last, char *line, int *num, char **rest)
{
	char			*temp;

	if (last->type == PIPE || last->type == AND || last->type == OR)
	{
		store_line(line, 0);
		temp = readline("> ");
		(*num)++;
		if (temp == NULL)
		{
			printf("minishell: syntax error: unexpected end of file\n");
			printf("exit\n");
			exit(1);
		}
		*rest = ft_strjoin(" ", *rest, SOUJAOUR);
		*rest = ft_strjoin(*rest, temp, SOUJAOUR);
		free(temp);
		return (1);
	}
	store_line(line, 1);
	return (0);
}

t_ast	*parse_line(char *line, t_chain **list, int *num)
{
	t_chain	*post;
	char	*rest;

	rest = NULL;
	convert_str(line, list);
	if (*list == NULL)
		return (NULL);
	tokenize_list(*list);
	check_syntax(*list, line, 0, 0);
	if (open_heredocs(*list, *num))
	{
		add_history(line);
		return (NULL);
	}
	prioritize_list(*list);
	join_redirs(*list);
	join_commands(*list, NULL, NULL);
	*list = assign_inputs(*list, NULL);
	if (complete_line(lstlast(*list), line, num, &rest))
		return (parse_line(rest, list, num));
	post = convert_infix(*list, NULL, NULL);
	return (build_tree(post));
}
