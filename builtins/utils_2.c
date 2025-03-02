/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkolani <bkolani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:07:14 by bkolani           #+#    #+#             */
/*   Updated: 2025/03/02 21:39:52 by bkolani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	*create_new_env(const char *line)
{
	char	**splited_line;
	t_env	*new_env;

	new_env = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
	splited_line = ft_split(line, '=', BKOLANI);
	new_env->key = ft_strdup(splited_line[0], BKOLANI);
	if (splited_line[1])
		new_env->value = ft_strdup(splited_line[1], BKOLANI);
	new_env->full = ft_strdup(line, BKOLANI);
	new_env->next = NULL;
	return (new_env);
}

t_env	*handle_env(char **envp)
{
	int		i;
	t_env	*head;
	t_env	*new;

	i = -1;
	head = NULL;
	while (envp[++i])
	{
		new = create_new_env(envp[i]);
		ft_lstadd_back_env(&head, new);
	}
	return (head);
}

t_env	*get_env_var(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	check_env_str(const char *line, char **str_tab)
{
	int	i;

	i = -1;
	while (str_tab[0][++i])
	{
		if (((str_tab[0][0] >= '0' && str_tab[0][0] <= '9')
			&& str_tab[0][ft_strlen(str_tab[0]) - 1])
			|| (!(str_tab[0][i] >= 'a' && str_tab[0][i] <= 'z')
			&& !(str_tab[0][i] >= 'A' && str_tab[0][i] <= 'Z')
			&& !(str_tab[0][i] >= '0' && str_tab[0][i] <= '9')
			&& str_tab[0][i] != '_' && !str_tab[1]))
		{
			printf("Minishell: export: `%s': not a valid identifier\n", line);
			return (1);
		}
	}
	return (0);
}
