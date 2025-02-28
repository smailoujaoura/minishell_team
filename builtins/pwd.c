/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:38 by bkolani           #+#    #+#             */
/*   Updated: 2025/02/28 18:29:26 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    builtin_pwd(t_shell *mini)
{
	char    *path;
	t_env	*pwd_str;

	path = getcwd(NULL, 0);
	pwd_str = NULL;
	if (!path)
	{
		pwd_str = get_env_var(mini->env, "PWD");
		printf("%s\n", pwd_str->value);
		// perror("pwd");
		return ;
	}
	printf("%s\n", path);
	free(path);
}
