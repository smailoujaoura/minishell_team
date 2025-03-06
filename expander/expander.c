/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 12:44:57 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/06 16:36:47 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// copies part of a string that is not part of a variable and joins it
// to the other parts that might have been expanded
char	*expand_str(char *str, int i, char **flags, t_shell *mini)
{
	char	*new;
	char	*value;
	int		singles;
	int		doubles;

	new = NULL;
	singles = 0;
	doubles = 0;
	while (str[i])
	{
		if (!singles && str[i] == '$' && is_var('$', str[i + 1], STRT))
		{
			mini->doubles = doubles;
			mini->singles = singles;
			value = get_value(str, &i, mini, is_var('$', str[i + 1], STRT));
			new = handle_var_values(value, new, flags, doubles);
		}
		else
		{
			value = just_copy(str, &i, &singles, &doubles);
			construct_flags(value, LITERAL, flags);
			new = ft_strjoin(new, value, SOUJAOUR);
		}
	}
	return (new);
}

// expands a command or its arguments if they contain $ or *
char	**expand_cmd(t_chain *cmd, t_argv *args, t_shell *mini)
{
	char	*temp;
	char	*flags;
	char	*actual;

	flags = NULL;
	actual = expand_str(cmd->content, 0, &flags, mini);
	while (args)
	{
		flags = ft_strjoin(flags, "s", SOUJAOUR);
		actual = ft_strjoin(actual, " ", SOUJAOUR);
		temp = expand_str(args->content, 0, &flags, mini);
		actual = ft_strjoin(actual, temp, SOUJAOUR);
		args = args->next;
	}
	if (actual == NULL)
		return (NULL);
	flags = ultimate(actual, flags, 0, 0);
	flags = remove_flags(flags, &actual, actual);
	return (ultimate_split(actual, flags, SPLIT, NULL));
}

// expand a filename if it has $ or *
char	**expand_files(t_chain *node, t_shell *mini)
{
	char	*actual;
	char	*flags;

	flags = NULL;
	actual = expand_str(node->file, 0, &flags, mini);
	if (actual == NULL)
		return (NULL);
	flags = ultimate(actual, flags, 0, 0);
	flags = remove_flags(flags, &actual, actual);
	return (ultimate_split(actual, flags, SPLIT, NULL));
}

// this function will expand redirs's variables $ and wildcards *
// it also expands the heredoc's contents
void	expand_redirs(t_chain *ptr, t_shell *mini)
{
	char	*new_name;
	char	**result;

	while (ptr)
	{
		if (ptr->type != HEREDOC)
		{
			result = expand_files(ptr, mini);
			if (result == NULL || result[1] != NULL)
			{
				printf("minishell: %s: ambiguous redirect\n", ptr->file);
				ptr->ambiguous = 1;
				return ;
			}
			ptr->file = result[0];
		}
		else
		{
			new_name = generate_random_name();
			expand_heredoc(ptr, mini, new_name, ptr->fd);
		}
		ptr = ptr->next;
	}
}
