/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:38 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/10 14:50:01 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*store_pwd(char *store_it, int flag)
{
	static char	*pwd;

	if (flag == -1)
	{
		pwd = store_it;
	}
	return (pwd + 4);
}

void	builtin_pwd(t_shell *mini)
{
	char	*path;
	t_env	*pwd_str;

	path = getcwd(NULL, 0);
	pwd_str = NULL;
	if (!path)
	{
		pwd_str = get_env_var(mini->env, "PWD");
		if (pwd_str == NULL)
			printf("%s\n", store_pwd(NULL, 2));
		else
			printf("%s\n", pwd_str->value);
		return ;
	}
	printf("%s\n", path);
	free(path);
}
