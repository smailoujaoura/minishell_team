/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:46:07 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/06 14:55:46 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_empty_cmd(const char *cmd, t_shell *mini)
{
	if (ft_strncmp(cmd, "", SIZE_MAX) == 0)
	{
		printf("Command '' not found,  but can be installed with:\n"
			"apt install mailutils-mh  # version 1:3.14-1, or\n"
			"apt install mmh           # version 0.4-4\n"
			"apt install nmh           # version 1.7.1-11\n"
			"apt install termtris      # version 1.3-1ubuntu1\n"
			"Ask your administrator to install one of them.\n");
		mini->last_exit = 127;
		return (1);
	}
	return (0);
}
