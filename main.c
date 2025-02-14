/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:11:21 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/14 13:35:54 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*bkolani(char *argv[], char *envp[]);
void	loop_minishell(t_env *env);

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;
	t_env	*env;

	env = bkolani(argv, envp);
	loop_minishell(env);
}