/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:56:07 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/14 11:09:23 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// open 
int	open_files(char *new, int *expand_fd)
{
	int	expand_read_fd;

	*expand_fd = open(new, O_WRONLY | O_CREAT, 0600);
	if (*expand_fd == -1)
		raise_error(strerror(errno));
	expand_read_fd = open(new, O_RDONLY);
	if (expand_read_fd == -1)
		raise_error(strerror(errno));
	unlink(new);
	return (expand_read_fd);
}

// checks if current string is variable
int	check_var_existence(char current, char next)
{
	if (current == '$')
	{
		if (next == '?')
			return (1);
		else if (ft_isalpha(next) || next == '_')
			return (2);
	}
	return (0);
}

// find a variable key(key=value)
char	*get_var_key(char *key, int *i)
{
	int	start;

	start = *i;
	while (key[*i] && check_var_existence('$', key[*i]))
	{
		(*i)++;
	}
	return (ft_substr(&key[start], 0, (*i) - start, SOUJAOUR));
}

// copies parts of lines and joins them with others expanded or literals 
char	*just_copy_until(char *line, t_shell *mini, int i)
{
	char	*value;
	char	*new_line;
	int		singles;
	int		doubles;

	new_line = NULL;
	while (line[i])
	{
		doubles = 30;
		singles = 0;
		if (line[i] == '$' && is_var('$', line[i + 1], STRT))
		{
			value = get_value(line, &i, mini, is_var('$', line[i + 1], STRT));
			new_line = ft_strjoin(new_line, value, SOUJAOUR);
		}
		else
		{
			value = just_copy(line, &i, &singles, &doubles);
			new_line = ft_strjoin(new_line, value, SOUJAOUR);
		}
	}
	return (new_line);
}

// expands heredoc contents 
void	expand_heredoc(t_chain *ptr, t_shell *mini, char *new, int source_fd)
{
	char	*line;
	char	*new_line;
	int		expand_fd;

	if (ptr->delim_in_quotes)
		return ;
	else
	{
		ptr->fd = open_files(new, &expand_fd);
		line = get_next_line(source_fd);
		while (line)
		{
			new_line = just_copy_until(line, mini, 0);
			write(expand_fd, new_line, ft_strlen(new_line));
			free(line);
			line = get_next_line(source_fd);
		}
		close(source_fd);
		close(expand_fd);
	}
}
