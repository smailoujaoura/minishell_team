/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:28:44 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/27 11:13:33 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	finish_exec(t_ast *tree)
{
	t_chain	*ptr;

	ptr = tree->data->adj_f;
	while (ptr)
	{
		if (ptr->fd > 2)
			close(ptr->fd);
	}
}

int	ft_open(char *path, int mode, int permissions)
{
	int fd;

	fd = open(path, mode, permissions);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", path, strerror(errno));
	}
	return (fd);
}

void	assign_fds_builtins(t_ast *tree, int action)
{
	static int	original_in;
	static int	original_out;

	if (action)
	{
		original_in = dup(STDIN_FILENO);
		original_out = dup(STDOUT_FILENO);
		if (tree->in_fd != -1)
		{
			dup2(tree->in_fd, STDIN_FILENO);
			close(tree->in_fd);
		}
		if (tree->out_fd != -1)
		{
			dup2(tree->out_fd, STDOUT_FILENO);
			close(tree->out_fd);
		}
	}
	else
	{
		dup2(original_in, STDIN_FILENO);
		dup2(original_out, STDOUT_FILENO);
	}	
}

void	assign_fds(t_ast *tree)
{
	t_chain	*ptr;

	ptr = tree->data->adj_f;
	while (ptr)
	{
		if (ptr->type == REDIR_APPEND || ptr->type == REDIR_OUT)
			tree->out_fd = ptr->fd;
		if (ptr->type == REDIR_IN)
			tree->in_fd = ptr->fd;
		if (ptr->type == HEREDOC)
			tree->in_fd = ptr->fd;
		ptr = ptr->next;
	}
}

bool	create_adj_files(t_chain *adj)
{
	t_chain	*ptr;

	ptr = adj;
	while (ptr)
	{
		if (ptr->ambiguous)
			return (false);
		if (ptr->type != HEREDOC)
		{
			if (ptr->type == REDIR_OUT)
				ptr->fd = ft_open(ptr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (ptr->type == REDIR_APPEND)
				ptr->fd = ft_open(ptr->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else if (ptr->type == REDIR_IN)
				ptr->fd = ft_open(ptr->file, O_RDONLY , 0644);
		}
		if (ptr->fd == -1)
			return (false);
		ptr = ptr->next;
	}
	return (true);
}
