/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 09:02:47 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/02 15:24:32 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	open_redir(t_chain *redir)
{
	if (redir->ambiguous)
		return ;
	if (redir->type == REDIR_IN)
		redir->fd = open(redir->file, O_RDONLY);
	if (redir->type == REDIR_OUT)
		redir->fd = open(redir->file, O_WRONLY | O_CREAT, 0644);
	if (redir->type == REDIR_APPEND)
		redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->fd == -1)
	{
		dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("minishell: %s: %s\n", redir->file, strerror(errno));
	}
}

int	open_and_assign(t_chain *redirs)
{
	while (redirs)
	{
		open_redir(redirs);
		if (redirs->ambiguous || redirs->fd == -1)
		{
			return (1);
		}
		if (redirs->type == HEREDOC || redirs->type == REDIR_IN)
		{
			dup2(redirs->fd, STDIN_FILENO);
		}
		if (redirs->type == REDIR_APPEND || redirs->type == REDIR_OUT)
		{
			dup2(redirs->fd, STDOUT_FILENO);
		}
		close(redirs->fd);
		redirs = redirs->next;
	}
	return (0);
}

int	assign_fds_builtins(t_ast *tree, char *cmd, int action)
{
	static int	original_in;
	static int	original_out;

	if (ft_strncmp("exit", cmd, SIZE_MAX) == 0)
		return (0);
	if (action)
	{
		original_in = dup(STDIN_FILENO);
		original_out = dup(STDOUT_FILENO);
		if (open_and_assign(tree->data->adj_f))
			return (1);
	}
	else
	{
		dup2(original_in, STDIN_FILENO);
		if (!isatty(original_in))
			close(original_in);
		dup2(original_out, STDOUT_FILENO);
		if (!isatty(original_out))
			close(original_out);
	}
	return (0);
}
