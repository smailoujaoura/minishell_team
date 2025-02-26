/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:10 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/26 11:07:03 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	complete_line(t_chain *last, char *line, int *num, char **rest)
{
	char			*temp;
	static char		*history;

	if (last->type == PIPE || last->type == AND || last->type == OR)
	{
		temp = readline("> ");
		(*num)++;
		if (temp == NULL)
		{
			printf("minishell: syntax error: unexpected end of file\n");
			printf("exit\n");
			exit(1);
		}
		*rest = ft_strdup(temp, SOUJAOUR);
		history = ft_strjoin(history, " ", SOUJAOUR);
		history = ft_strjoin(line, temp, SOUJAOUR);
		free(temp);
		return (1);
	}
	if (history)
		add_history(history);
	else if (line[0])
		add_history(line);
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
	join_commands(*list);
	*list = assign_inputs(*list, NULL);

	if (complete_line(lstlast(*list), line, num, &rest))
		return (parse_line(rest, list, num));
	post = convert_infix(*list);
	return (build_tree(post));
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

void	loop_minishell(t_shell *mini)
{
	t_chain	*list;
	t_ast	*root;
	char	*line;
	int		num;
	
	num = 0;
	list = NULL;
	while (1337)
	{
		line = readline("Minishell: ");
		if (line == NULL)
			break ;
		root = parse_line(line, &list, &num);
		executor(root, mini);
		free(line);
		ft_malloc(0, DEALLOCATE);
	}
	ft_malloc_bkol(0, DEALLOCATE);
}
