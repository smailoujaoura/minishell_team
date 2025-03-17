/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:10 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 09:50:30 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

// adds a line to the history of readline history library as needed
int	store_line(char *new, int flag)
{
	static char	*store;

	store = ft_strjoin(store, new, SOUJAOUR);
	if (flag == 1)
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

	rest = NULL;
	convert_str(line, list);
	if (*list == NULL)
		return (NULL);
	tokenize_list(*list);
	mini->error = check_syntax(*list, line, 0, 0);
	if (open_heredocs(*list, mini) || mini->error == -1)
	{
		if (mini->error == -1)
			mini->last_exit = 2;
		store_line(line, 0);
		store_line(NULL, 1);
		return (NULL);
	}
	prioritize_list(*list);
	join_redirs(*list);
	join_commands(*list, NULL, NULL);
	*list = assign_inputs(*list);
	if (complete_line(mini, lstlast(*list), line, &rest) && mini->flag != 1)
		return (parse_line(rest, list, mini));
	post = convert_infix(*list, NULL, NULL);
	return (build_tree(post));
}

void	minishell(t_shell *mini, struct termios *initial)
{
	t_chain			*list;
	t_ast			*root;
	char			*line;

	while (1337)
	{
		list = NULL;
		mini->flag = 0;
		mini->error = 0;
		setup_signals(1);
		line = readline("Minishell:$ ");
		if (line == NULL)
			break ;
		root = parse_line(line, &list, mini);
		free(line);
		setup_signals(2);
		if (!mini->flag)
			executor(root, mini);
		ft_malloc(0, DEALLOCATE);
		mini->num++;
		mini->volatile_exit = 0;
		if (tcsetattr(STDIN_FILENO, TCSANOW, initial) < 0)
			return ;
	}
	ft_malloc_bkol(0, DEALLOCATE);
}
