/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 09:02:47 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/10 15:41:16 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	open_redir(t_chain *redir)
{
	char	*error;

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
		write(2, "minishell: ", 12);
		write(2, redir->file, ft_strlen(redir->file));
		write(2, ": ", 3);
		error = strerror(errno);
		write(2, error, ft_strlen(error));
		write(2, "\n", 2);
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
			ft_dup2(redirs->fd, STDIN_FILENO);
		}
		if (redirs->type == REDIR_APPEND || redirs->type == REDIR_OUT)
		{
			ft_dup2(redirs->fd, STDOUT_FILENO);
		}
		close(redirs->fd);
		redirs = redirs->next;
	}
	return (0);
}

int	reset_orig_fds(int orig_in, int orig_out)
{
	ft_dup2(orig_in, STDIN_FILENO);
	close(orig_in);
	ft_dup2(orig_out, STDOUT_FILENO);
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
		original_in = ft_dup(STDIN_FILENO);
		original_out = ft_dup(STDOUT_FILENO);
		if (open_and_assign(tree->data->adj_f))
		{
			close(original_in);
			close(original_out);
			return (1);
		}
	}
	else
		reset_orig_fds(original_in, original_out);
	return (0);
}
