/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 20:30:57 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/10 15:26:31 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	run_or(t_ast *tree, t_shell *mini)
{
	((tree->f == 1) && (tree->left->f = 1) && (tree->right->f = 1));
	executor(tree->left, mini);
	mini->volatile_exit = mini->last_exit;
	if (mini->last_exit != 0)
		executor(tree->right, mini);
	mini->volatile_exit = mini->last_exit;
}

void	run_and(t_ast *tree, t_shell *mini)
{
	((tree->f == 1) && (tree->left->f = 1) && (tree->right->f = 1));
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
		write(2, "Command '' not found\n", 22);
		return (1);
	}
	return (0);
}

void	panic_exit(char *ptr, int custom)
{
	if (custom)
	{
		printf("%s", ptr);
	}
	else
		perror("Minishell exit");
	ft_malloc(0, DEALLOCATE);
	ft_malloc_bkol(0, DEALLOCATE);
	exit(custom);
}
