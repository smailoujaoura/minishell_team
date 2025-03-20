/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:56:10 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/20 21:16:58 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_shell(char **envp, t_shell *mini)
{
	char			*pwd;

	mini->env = handle_env(envp, -1, NULL, mini);
	mini->last_exit = 0;
	mini->volatile_exit = 0;
	mini->flag = 0;
	mini->export = 0;
	mini->doubles = 0;
	mini->singles = 0;
	mini->num = 1;
	pwd = getcwd(NULL, 0);
	if (pwd)
		store_pwd(ft_strdup(pwd, BKOLANI), -1);
	else
	{
		perror("shell-init: error retrieving current directory: getcwd: \
cannot access parent directories");
	}
	free(pwd);
}

// Print env vars
void	builtin_env(t_env *env)
{
	if (!env)
		return ;
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
