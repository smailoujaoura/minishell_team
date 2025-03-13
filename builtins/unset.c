/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:58:05 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/13 12:48:32 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_unset(t_shell *mini, char **argv)
{
	int		i;
	t_env	*target;
	t_env	*position;

	i = 0;
	target = NULL;
	if (!argv[1])
		return ;
	while (argv[++i])
	{
		target = get_env_var(mini->env, argv[i]);
		if (target != NULL && target != mini->env)
		{
			position = mini->env;
			while (position && position->next != target)
				position = position->next;
			position->next = target->next;
		}
		else if (target == mini->env)
		{
			mini->env = target->next;
		}
	}
}
