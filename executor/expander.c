/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 12:44:57 by soujaour          #+#    #+#             */
/*   Updated: 2025/02/27 17:44:43 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

# define RECORD 9327
# define SORT 1232

int	is_var(char *str, int i)
{
	if (str[i] == '$' && (str[i + 1] == '_' || ft_isalpha(str[i + 1])))
	{
		return (1);
	}
	return (0);
}

void	singles_doubles(int *singles, int *doubles, char c)
{
	if (c == '\'' && !*singles && !*doubles)
	{
		*singles = 1;
	}
	else if (c == '\'' && *singles)
	{
		*singles = 0;
	}	
	if (c == '"' && !*doubles && !*singles)
	{
		*doubles = 1;
	}	
	else if (c == '"' && *doubles)
	{
		*doubles = 0;
	}
}

char	*just_copy(char *str, int *i, int *singles, int *doubles)
{
	char	*new;
	int		len;

	len = 0;
	new = NULL;
	while (str[*i])
	{
		singles_doubles(singles, doubles, str[*i]);
		if (!singles && str[*i] == '$')
			break ;
		else
		{
			while (str[*i + len] && (*singles || !is_var(str, *i + len) ))
			{
				len++;
				singles_doubles(singles, doubles, str[*i + len]);
			}
			new = ft_substr(str, *i, len, SOUJAOUR);
			(*i) += len;
			return (new);
		}
	}
	return (new);
}

char	*expand_var(char *str, int *i, int *singles, int *doubles)
{
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while (str[*i + len])
	{
		singles_doubles(singles, doubles, str[*i + len]);
		if (!*singles && str[*i + len] == '$')
		{
			while (str[*i + len + 1] && (str[*i + len + 1] == '_' || ft_isalnum(str[*i + len + 1])))
			{
				len++;
			}
			value = ft_substr(str, *i + 1, len, SOUJAOUR);
			(*i) += ++len;
			break ;
		}
		else
			break ;
	}
	return (value);
}

char	*get_value_wrapper(char *var, t_env *env)
{
	t_env	*node;


	node = get_env_var(env, var);
	if (node == NULL)
		return (ft_strdup("", SOUJAOUR));
	else
		return (node->value);
}

void	construct_flags(char *str, char flag, char **flags)
{
	char	*new_flag;
	size_t	len;
	size_t	i;

	len = ft_strlen(str);
	new_flag = ft_calloc(len + 1, sizeof(char), SOUJAOUR);
	i = 0;
	while (i < len)
	{
		new_flag[i] = flag;
		i++;
	}
	new_flag[i] = '\0';
	if (*flags == NULL)
		*flags = new_flag;
	else
		*flags = ft_strjoin(*flags, new_flag, SOUJAOUR);
}

char	*store_wilds(char *wilds, int index, int action)
{
	char		**arr;
	static char	*last;

	if (action == STORE)
	{
		last = wilds;
	}
	if (action == RETRIEVE)
	{
		if (index != -1)
		{
			arr = ft_split(last, SPLIT, SOUJAOUR);
			return (arr[index]);
		}
		return (last);
	}
	return (NULL);
}

char	*create_wilds(char *wilds)
{
	int		j;
	int		i;
	int		len;
	char	*new_wilds;

	len = 0;
	i = 0;
	while (wilds[i])
	{
		if (wilds[i] != QUOTE)
			len++;
		i++;
	}
	new_wilds = ft_calloc(len + 1, 1, SOUJAOUR);
	i = 0;
	j = 0;
	while (j < len)
	{
		if (wilds[i] != QUOTE)
			new_wilds[j++] = wilds[i];
		i++;
	}
	return (store_wilds(new_wilds, 0, STORE), wilds);
}

int	inside_level_two(char *str, char *sources, int i)
{
	if (str[i] == '"' || str[i] == '\'')
	{
		while (sources[i] == FROM_VAR)
		{
			i++;
			if ((str[i] == '"' || str[i] == '\'') && sources[i] == FROM_VAR)
				return (1);
		}
	}
	return (0);
}

char	*wild_shell(char *sources, char *quotes, char *str)
{
	int		inside_lvl_one;
	int		inside_lvl_two;
	int		i;

	i = 0;
	while (sources[i])
	{
		if (sources[i] == SPLIT)
			quotes[i] = SPLIT;
		i++;
	}

	i = 0;
	inside_lvl_one = 0;
	inside_lvl_two = 0;
	while (quotes[i])
	{
		if (!inside_lvl_one && quotes[i] == QUOTE)
			inside_lvl_one++;
		else if (inside_lvl_one && quotes[i] == QUOTE)
			inside_lvl_one--;

		if (!inside_lvl_two && inside_level_two(str, sources, i))
			inside_lvl_two++;
		else if (inside_lvl_two && (str[i] == '"' || str[i] == '\''))
			inside_lvl_two--;
			
		if (str[i] == '*' && !inside_lvl_one && sources[i] == LITERAL)
			quotes[i] = IS_WILD;
		else if (str[i] == '*' && !inside_lvl_two && !inside_lvl_one)
			quotes[i] = IS_WILD;
		else if (quotes[i] != SPLIT && quotes[i] != QUOTE)
			quotes[i] = NOT_WILD;
		i++;
	}
	return (create_wilds(quotes));
}


char	*copy_ifnot_quote(char *str, char *quotes_flag, char *new, char *src_flag)
{
	int		i;
	int		k;
	char	*ultimate_flags;

	i = 0;
	k = 0;
	ultimate_flags = wild_shell(src_flag, quotes_flag, str);
	while (str[i])
	{
		if (ultimate_flags[i] != QUOTE)
		{
			new[k] = str[i];
			k++;
		}
		i++;
	}
	new[k] = '\0';
	return (new);
}

int	count_remainings(char *quotes_flag)
{
	int i;
	int	len;

	i = 0;
	len = 0;
	while (quotes_flag[i])
	{
		if (quotes_flag[i] == NOT_QUOTE)
			len++;
		i++;
	}
	return (len);
}

void	assign_flag(char *edit, char which, int one, int two)
{
	if (which == '"' && one == 0)
		*edit = QUOTE;
	else if (which == '\'' && two == 0)
		*edit = QUOTE;
	else
		*edit = NOT_QUOTE;
}

char	*remove_quotes(char *str, char *flags, int one, int two)
{
	char	*quotes_flag;
	char	*new;
	int		i;

	quotes_flag = ft_calloc(ft_strlen(str) + 1, 1, SOUJAOUR);
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' && one != 1 && flags[i] == LITERAL)
			two++;
		if (str[i] == '\'' && two != 1 && flags[i] == LITERAL)
			one++;
		if (two == 2)
			two = 0;
		if (one == 2)
			one = 0;
		if (flags[i] == LITERAL && (str[i] == '"' || str[i] == '\''))
			assign_flag(&quotes_flag[i], str[i], one, two);
		else
			quotes_flag[i] = NOT_QUOTE;
		i++;
	}
	new = ft_malloc(count_remainings(quotes_flag) + 1, ALLOCATE);
	return (copy_ifnot_quote(str, quotes_flag, new, flags));
}

int	match_wildcard(const char *pattern, const char *str, const char *is_wild)
{
	while (*pattern)
	{
		if (*pattern == '*' && *is_wild == IS_WILD)
		{
			while (*pattern == '*' && *is_wild == IS_WILD)
				pattern++, is_wild++;
			if (!*pattern)
				return (1);
			while (*str)
				if (match_wildcard(pattern, str++, is_wild))
					return (1);
			return (0);
		}
		if (*pattern != *str || (*pattern == '*' && *is_wild == NOT_WILD))
			return (0);
		pattern++, str++, is_wild++;
	}
	return (*str == '\0');
}

char	**dynamic_array(char **arr, char *new)
{
	char	**result;
	int		size;

	size = 0;
	while (arr && arr[size])
	{
		size++;
	}
	result = ft_calloc(size + 2, sizeof(char *), SOUJAOUR);
	size = 0;
	while (arr && arr[size])
	{
		result[size] = arr[size];
		size++;
	}
	result[size] = ft_strdup(new, SOUJAOUR);
	return (result);
}

void	bubble_ascii_sort(char **arr)
{
	char	*tmp;
	int		swapped;
	int		i;

	swapped = 1;
	while (swapped)
	{
		i = 0;
		swapped = 0;
		while (arr[i + 1])
		{
			if (ft_strncmp(arr[i], arr[i + 1], INT32_MAX) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

void	sort_newly_added(int action, char **arr)
{
	static int	starting_point;

	if (action == RECORD)
	{
		while (arr && arr[starting_point])
			starting_point++;
	}
	else if (action == SORT)
	{
		bubble_ascii_sort(&arr[starting_point]);
		starting_point = 0;
	}
}

char	**expand_wildcards(char **arr, char **result, char *is_wild, int i)
{
	DIR				*dir;
	struct dirent	*entry;
	int				found;

	while (arr[i])
	{
		found = 0;
		is_wild = store_wilds(0, i, RETRIEVE);
		if (ft_strchr(is_wild, IS_WILD))
		{
			dir = opendir(".");
			entry = readdir(dir);
			sort_newly_added(RECORD, result);
			while (entry)
			{
				if (match_wildcard(arr[i], entry->d_name, is_wild))
				{
					result = dynamic_array(result, entry->d_name);
					found = 1;
				}	
				entry = readdir(dir);
			}
			if (found == 0)
				result = dynamic_array(result, arr[i]);
			sort_newly_added(SORT, result);
			closedir(dir);
		}
		else
			result = dynamic_array(result, arr[i]); 
		i++;
	}
	return (result);
}

int	is_spaces(char *str)
{
	int	i;

	i = 0;
	if (ft_strlen(str) == 0)
		return (1);
	while (str[i])
	{
		if (str[i] != ' ' && !(str[i] >= 8 && str[i] <= 13))
			return (0);
	}
	return (1);
}

char	*handle_var_values(char *value, char *new, char **flags)
{
	char	**var_values;
	int		i;

	i = 0;
	var_values = ft_split(value, ' ', SOUJAOUR);
	while (var_values[i])
	{
		new = ft_strjoin(new, var_values[i], SOUJAOUR);
		construct_flags(var_values[i], FROM_VAR, flags);
		if (var_values[i + 1])
		{
			new = ft_strjoin(new, " ", SOUJAOUR);
			*flags = ft_strjoin(*flags, "s", SOUJAOUR);
		}
		i++;
	}
	return (new);
}

char	*expand_str(char *str, int i, char **flags, t_env *env)
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
		if (is_var(str, i))
		{
			value = expand_var(str, &i, &singles, &doubles);
			value = get_value_wrapper(value, env);
			new = handle_var_values(value, new, flags);
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

int	count_words(char *delims, char sep)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (delims[i])
	{
		if (delims[i] != sep && (delims[i + 1] == sep || !delims[i + 1]))
			counter++;
		i++;
	}
	return (counter);
}

char	**ft_split_if(char *str, char *delims, char sep)
{
	int		i;
	int		words;
	char	*trail;
	char	**arr;

	i = 0;
	words = count_words(delims, sep);
	arr = ft_calloc(words + 1, sizeof(char *), SOUJAOUR);
	i = 0;
	while (i < words)
	{
		while (*delims == sep && *str)
		{
			str++;
			delims++;
		}
		trail = delims;
		while (*delims && *delims != sep)
			delims++;
		arr[i] = ft_calloc(delims - trail + 1, 1, SOUJAOUR);
		ft_strlcpy(arr[i], str, delims - trail + 1);
		str += delims - trail;
		i++;
	}
	return (arr);
}

char	**expand_cmd(t_chain *cmd, t_argv *args, t_env *env)
{
	char	*temp;
	char	*flags;
	char	*actual;
	char	**arr;

	flags = NULL;
	actual = expand_str(cmd->content, 0, &flags, env);
	while (args)
	{
		flags = ft_strjoin(flags, "s", SOUJAOUR);
		actual = ft_strjoin(actual, " ", SOUJAOUR);
		temp = expand_str(args->content, 0, &flags, env);
		actual = ft_strjoin(actual, temp, SOUJAOUR);
		args = args->next;
	}
	if (actual == NULL)
		return (NULL);
	actual = remove_quotes(actual, flags, 0, 0);
	arr = ft_split_if(actual, store_wilds(NULL, -1, RETRIEVE), SPLIT);
	return (expand_wildcards(arr, NULL, NULL, 0));
}

char	**expand_files(t_chain *node, t_env *env)
{
	char	*var_values;
	char	*flags;
	char	**result;

	flags = NULL;
	var_values = expand_str(node->file, 0, &flags, env);
	var_values = remove_quotes(var_values, flags, 0, 0);
	result = ft_split_if(var_values, store_wilds(NULL, -1, RETRIEVE), SPLIT);
	return (expand_wildcards(result, NULL, NULL, 0));
}

int	open_files(char *new, int *expand_fd)
{
	int	expand_read_fd;

	*expand_fd = open(new, O_WRONLY | O_CREAT, 0600);
	if (*expand_fd == -1)
		panic_exit(strerror(errno), 9273112);
	expand_read_fd = open(new, O_RDONLY);
	if (expand_read_fd == -1)
		panic_exit(strerror(errno), 982723);
	unlink(new);
	return (expand_read_fd);	
}

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

char	*just_copy_until(char *line, t_shell *mini)
{
	int		i;
	int		start;
	char	*key;
	char	*value;
	char	*new_line;

	i = 0;
	new_line = NULL;
	while (line[i])
	{
		if (check_var_existence(line[i], line[i + 1]))
		{
			i++;
			key = get_var_key(line, &i);
			value = get_value_wrapper(key, mini->env);
			new_line = ft_strjoin(new_line, value, SOUJAOUR);
		}
		else
		{
			start = i;
			i++;
			while (line[i] && !check_var_existence(line[i], line[i + 1]))
				i++;
			value = ft_substr(&line[start], 0, i - start, SOUJAOUR);
			new_line = ft_strjoin(new_line, value, SOUJAOUR);
		}
	}
	return (new_line);
}

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
			new_line = just_copy_until(line, mini);
			write(expand_fd, new_line, ft_strlen(new_line));
			free(line);
			line = get_next_line(source_fd);
		}
		close(source_fd);
		close(expand_fd);
	}
}

int	just_open_old_name(char *old)
{
	int fd;

	fd = open(old, O_RDONLY);
	if (fd == -1)
		panic_exit("Temp file removed!", 2738271);
	return (fd);
}

void	expand_redirs(t_chain *ptr, t_shell *mini)
{
	char	*new_name;
	char	**result;

	while (ptr)
	{
		if (ptr->type != HEREDOC)
		{
			result = expand_files(ptr, mini->env);
			if (result[1] != NULL) // || is_spaces(result[0])
			{
				printf("minishell: %s: ambiguous redirect\n", ptr->file);
				ptr->ambiguous = 1;
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

// add to exit status when signal some integer 128, wh?
// $var ls
// unlink the file when 
// ambiguous redirect when var expands to nothing