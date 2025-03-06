/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 09:02:47 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 13:21:15 by bkolani          ###   ########.fr       */
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
		ft_dup2(STDERR_FILENO, STDOUT_FILENO);
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

int	is_a_dir(const char *cmd, t_shell *mini)
{
	struct stat	file_info;

	if ((ft_strncmp(cmd, ".", 1) == 0 || ft_strncmp(cmd, "/", 1) == 0)
			&& stat(cmd, &file_info) == -1)
	{
		perror("minishell");
		mini->last_exit = 1;
		return (1);
	}
	else if ((ft_strncmp(cmd, ".", 1) == 0 || ft_strncmp(cmd, "/", 1) == 0)
			&& stat(cmd, &file_info) == 0)
	{
		printf("minishell: %s: Is a directory\n", cmd);
		mini->last_exit = 126;
		return (1);
	}
	return (0);
}
