/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 09:02:47 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/04 12:36:36 by soujaour         ###   ########.fr       */
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
		if (dup2(STDERR_FILENO, STDOUT_FILENO) < 0)
		{
			perror("minishell");
			return ;
		}
		printf("minishell: %s: %s\n", redir->file, strerror(errno));
	}
}

int	open_and_assign(t_chain *redirs)
{
	while (redirs)
	{
		open_redir(redirs);
		if (redirs->ambiguous || redirs->fd == -1)
			return (1);
		if (redirs->type == HEREDOC || redirs->type == REDIR_IN)
		{
			if (dup2(redirs->fd, STDIN_FILENO) < 0)
			{
				perror("minishell");
				return (1);
			}
		}
		if (redirs->type == REDIR_APPEND || redirs->type == REDIR_OUT)
		{
			if (dup2(redirs->fd, STDOUT_FILENO) < 0)
			{
				perror("minishell");
				return (1);
			}
		}
		close(redirs->fd);
		redirs = redirs->next;
	}
	return (0);
}

int	reset_orig_fds(int orig_in, int orig_out)
{
	if (dup2(orig_in, STDIN_FILENO) < 0)
	{
		perror("minishell");
		return (1);
	}
	if (!isatty(orig_in))
		close(orig_in);
	if (dup2(orig_out, STDOUT_FILENO) < 0)
	{
		perror("minishell");
		return (1);
	}
	if (!isatty(orig_out))
		close(orig_out);
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
		return (reset_orig_fds(original_in, original_out));
	return (0);
}
