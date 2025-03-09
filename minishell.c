/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:10 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/09 20:26:14 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	close_heredocs(t_chain *ptr)
{
	while (ptr)
	{
		if (ptr->type == HEREDOC)
		{
			close(ptr->fd);
		}
		ptr = ptr->back;
	}
}

// remove here-document's delimiter's quotes and open it, store its contents
int	open_heredocs(t_chain *list, int num)
{
	int	status;

	status = 0;
	count_heredocs(list);
	while (list)
	{
		if (list->error == -1)
			return (1);
		if (list->type == HEREDOC)
		{
			strip_heredoc(list, list->next->content);
			delete_any(list->next, 0);
			status = here_doc(list, num);
			if (status == EXIT_FAILURE)
			{
				close_heredocs(list);
				return (1);
			}
		}
		list = list->next;
	}
	return (0);
}

// adds a line to the history of readline history library as needed
void	store_line(char *new, int flag)
{
	static char	*store;

	store = ft_strjoin(store, new, SOUJAOUR);
	if (flag)
	{
		if (store[0])
			add_history(store);
		store = NULL;
	}
}

// continues reading if line is incomplete
int	complete_line(t_chain *last, char *line, int *num, char **rest)
{
	char	*temp;

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

// Using the Shunting Yard Algorithm, converst the line -> tokens list -> AST 
t_ast	*parse_line(char *line, t_chain **list, int *num, t_shell *mini)
{
	t_chain	*post;
	char	*rest;
	int		error;

	rest = NULL;
	convert_str(line, list);
	if (*list == NULL)
		return (NULL);
	tokenize_list(*list);
	error = check_syntax(*list, line, 0, 0);
	if (open_heredocs(*list, *num) || error == -1)
	{
		if (error == -1)
			mini->last_exit = 2;
		add_history(line);
		return (NULL);
	}
	prioritize_list(*list);
	join_redirs(*list);
	join_commands(*list, NULL, NULL);
	*list = assign_inputs(*list);
	if (complete_line(lstlast(*list), line, num, &rest))
		return (parse_line(rest, list, num, mini));
	post = convert_infix(*list, NULL, NULL);
	return (build_tree(post));
}
