/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:10 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/25 15:25:12 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	open_heredocs(t_chain *list)
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
			here_doc(list);
		list = list->next;
	}
	return (execute_or_not);
}

t_ast	*parse_line(char *line);

int	complete_line(t_chain *last, char **line)
{
	char		*temp;

	if (last->type == PIPE || last->type == AND || last->type == OR)
	{
		temp = readline("> ");
		if (temp == NULL)
		{
			printf("minishell: syntax error: unexpected end of file\n");
			printf("exit\n");
			exit(1);
		}
		*line = ft_strjoin(*line, " ", SOUJAOUR);
		*line = ft_strjoin(*line, temp, SOUJAOUR);
		free(temp);
		return (1);
	}
	else
	{
		if (line[0])
			add_history(*line);
		return (0);
	}
}

t_ast	*parse_line(char *line)
{
	t_chain	*list;
	t_chain	*last;
	t_chain	*post;
	t_ast	*root;

	list = convert_str(line);
	if (list == NULL)
		return (NULL);
	tokenize_list(list);
	check_syntax(list, line, 0, 0);
	prioritize_list(list);
	join_redirs(list);
	join_commands(list);
	if (open_heredocs(list))
		return (NULL);
	list = assign_inputs(list, NULL);
	last = lstlast(list);
	if (complete_line(last, &line))
		return (parse_line(line));
	post = convert_infix(list);
	root = build_tree(post);
	return (root);
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
	char	*line;
	t_ast	*root;
	
	while (1337)
	{
		line = readline("Minishell: ");
		if (line == NULL)
			break ;
		root = parse_line(line);
		executor(root, mini);
		free(line);
		ft_malloc(0, DEALLOCATE);
	}
	ft_malloc_bkol(0, DEALLOCATE);
}
