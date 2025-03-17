/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 20:30:57 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 11:02:24 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	run_or(t_ast *tree, t_shell *mini)
{
	if (tree->sub == 1)
	{
		tree->left->sub = 1;
		tree->right->sub = 1;
	}	
	executor(tree->left, mini);
	mini->volatile_exit = mini->last_exit;
	if (mini->last_exit != 0)
		executor(tree->right, mini);
	mini->volatile_exit = mini->last_exit;
}

void	run_and(t_ast *tree, t_shell *mini)
{
	if (tree->sub == 1)
	{
		tree->left->sub = 1;
		tree->right->sub = 1;
	}
	executor(tree->left, mini);
	mini->volatile_exit = mini->last_exit;
	if (mini->last_exit == 0)
		executor(tree->right, mini);
	mini->volatile_exit = mini->last_exit;
}

int	is_empty_command(char **argv, t_shell *mini)
{
	if (argv == NULL)
	{
		mini->last_exit = 0;
		return (1);
	}
	else if (!argv[0][0])
	{
		mini->last_exit = 127;
		write(2, "'': command not found\n", 23);
		return (1);
	}
	return (0);
}

void	raise_error(char *ptr)
{
	write(2, "Minishell: fatal error: ", 25);
	write(2, ptr, ft_strlen(ptr));
	ft_malloc(0, DEALLOCATE);
	ft_malloc_bkol(0, DEALLOCATE);
	exit(EXIT_FAILURE);
}
