/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:10 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/14 10:05:15 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

// adds a line to the history of readline history library as needed
int store_line(char *new, int flag)
{
	static char	*store;

	store = ft_strjoin(store, new, SOUJAOUR);
	if (flag)
	{
		if (store[0])
			add_history(store);
		store = NULL;
		return (0);
	}
	return (1);
}

// continues reading if line is incomplete
int	complete_line(t_shell *mini, t_chain *last, char *line, char **rest)
{
	char		*temp;

	if (last->type == PIPE || last->type == AND || last->type == OR)
	{
		store_line(line, 0);
		setup_signals(4);
		temp = readline("> ");
		setup_signals(5);
		mini->num++;
		if (mini->flag)
			return (store_line(NULL, 1));
		if (temp == NULL)
		{
			write(2, "minishell: syntax error: unexpected end of file\n", 49);
			write(2, "exit\n", 6);
			exit(1);
		}
		*rest = ft_strjoin(" ", *rest, SOUJAOUR);
		*rest = ft_strjoin(*rest, temp, SOUJAOUR);
		free(temp);
		return (1);
	}
	return (store_line(line, 1));
}

// Using the Shunting Yard Algorithm, converst the line -> tokens list -> AST 
t_ast	*parse_line(char *line, t_chain **list, t_shell *mini)
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
	if (open_heredocs(*list, mini) || error == -1)
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
	if (complete_line(mini, lstlast(*list), line, &rest))
		return (parse_line(rest, list, mini));
	post = convert_infix(*list, NULL, NULL);
	return (build_tree(post));
}
