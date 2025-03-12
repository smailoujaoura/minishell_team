/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:38 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/12 16:50:01 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*store_pwd(char *store_it, int flag)
{
	static char	*pwd;

	if (flag == -1)
	{
		pwd = ft_strdup(store_it, BKOLANI);
	}
	else if (flag == -2)
	{
		pwd = ft_strjoin(pwd, store_it, BKOLANI);
	}
	else if (flag == -3)
	{
		if (store_it[0] == '/')
			pwd = ft_strdup(store_it, BKOLANI);
		else
			pwd = ft_strjoin(pwd, store_it, BKOLANI);
	}
	if (pwd)
		return (pwd);
	else
		return (ft_strdup("/home/soujaour", BKOLANI));
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
		else if (pwd_str)
			printf("%s\n", pwd_str->value);
		return ;
	}
	printf("%s\n", path);
	free(path);
}


// Minishell:$ cd 1/2/3/4/
// Minishell:$ pwd
// /home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/1/2/3/4
// Minishell:$ pwd
// /home/soujaour/intra-uuid-f888ae00-4167-4221-b82b-55d615b58e2b-6428636-soujaour/1/2/3/4
// Minishell:$ unset PWD OLDPWD
// Minishell:$ pwd