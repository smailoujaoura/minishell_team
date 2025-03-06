/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 18:43:18 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/06 20:35:40 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_dir_or_file(const char *cmd)
{
	struct stat	file_info;

	if (stat(cmd, &file_info) < 0)
	{
		dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("minishell %s: %s\n", cmd, strerror(errno));
		return (127);
	}
	if (S_ISDIR(file_info.st_mode))
	{
		dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("minishell: %s: Is a directory\n", cmd);
		return (126);
	}
	else
	{
		if (access(cmd, X_OK) == -1)
		{
			printf("minishell %s: %s\n", cmd, strerror(errno));
			return (126);
		}
	}
	return (0);
}

int	is_a_dir(const char *cmd)
{
	if (ft_strlen(cmd) == 1 && ft_strncmp(cmd, ".", 1) == 0)
	{
		dup2(STDERR_FILENO, STDOUT_FILENO);
		printf("minishell: .: filename argument required\n"
			".: usage: . filename [arguments]\n");
		return (2);
	}
	if (((ft_strncmp(cmd, ".", 1) == 0 && ft_strchr(cmd, '/'))
			|| ft_strchr(cmd, '/')))
		return (check_dir_or_file(cmd));
	return (0);
}
