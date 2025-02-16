#include "../minishell.h"

int	is_var(char *str, int i)
{
	if (str[i] == '$' && (str[i + 1] == '_' || ft_isalnum(str[i + 1])))
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
			while (str[*i + len] && (*singles || str[*i + len] != '$'))
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

# define FROM_VAR '1'
# define LITERAL '2'

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
			construct_flags(value, FROM_VAR, flags);
			new = ft_strjoin(new, value, SOUJAOUR);
		}
		else
		{
			value = just_copy(str, &i, &singles, &doubles);
			construct_flags(value, LITERAL, flags);
			new = ft_strjoin(new, value, SOUJAOUR);
			// construct_flags();
		}
	}
	return (new);
}

# define IS_WILD 'w'
# define NOT_WILD 'n'
# define REMOVE_QUOTE 'r'
# define STORE 111
# define RETRIEVE 222

char	*store_last(char *wilds, int action)
{
	static char	*last;

	if (action == STORE)
		last = wilds;
	if (action == RETRIEVE)
		return (last);
	return (NULL);
}

void	create_wilds(char *wilds, char *flag)
{
	char	*last_wilds;
	int		new_len;
	int		i;
	int		j;

	i = 0;
	new_len = 0;
	while (wilds[i])
	{
		if (!flag[i])
			new_len++;
		i++;
	}
	i = 0;
	j = 0;
	last_wilds = ft_malloc(new_len + 1, ALLOCATE);
	while (wilds[i])
	{
		if (!flag[i])
			last_wilds[j++] = wilds[i];
		i++;
	}
	last_wilds[j] = '\0';
	store_last(last_wilds, STORE);
	// printf("expanded wil: [%s]\n", last_wilds);
}

void	wild_shell(char *flags, char *flag, char *str)
{
	int		inside;
	int		len;
	int		i;
	char	*wilds;

	i = 0;
	len = ft_strlen(flags);
	wilds = ft_malloc(len + 1, ALLOCATE);
	inside = 0;
	while (i < len)
	{
		if (!inside && flag[i])
			inside++;
		else if (inside && flag[i])
			inside--;
		if (str[i] == '*' && !inside && flags[i] == LITERAL)
			wilds[i] = IS_WILD;
		else
			wilds[i] = NOT_WILD;
		i++;
	}
	wilds[i] = '\0';
	create_wilds(wilds, flag);
	// i = 0;
	// printf("str :[%s]\n", str);
	// printf("wilds[%s]\n", wilds);
	// printf("flg :[%s]\n", flags);
	// printf("flag:[");
	// while ((size_t)i < ft_strlen(str))
	// {
	// 	printf("%c", flag[i] + '0');
	// 	i++;
	// }
	// printf("]\n");
	// printf("flag:[%s]\n", flags);
}

char	*copy_ifnot_quote(char *str, char *flag, char *new, char *flags)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	wild_shell(flags, flag, str);
	while (str[i])
	{
		if (flag[i] == 0)
		{
			new[k] = str[i];
			k++;
		}
		i++;
	}
	new[k] = '\0';
	return (new);
}

int	count_rems(char *s)
{
	int i;
	int	rems;

	i = 0;
	rems = 0;
	while (s[i])
	{
		if (s[i] == 1)
			rems++;
		i++;
	}
	return (rems);
}

void	assign_flag(char *flag, char c, int one, int two)
{
	if (c == '"' && one == 0 && (two == 1 || two == 0))
		*flag = 1;
	if (c == '\'' && two == 0 && (one == 1 || one == 0))
		*flag = 1;
}

char	*remove_quotes(char *str, char *flags, int one, int two)
{
	char	*flag;
	char	*new;
	int		i;

	flag = ft_calloc(ft_strlen(str) + 1, 1, SOUJAOUR);
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
			assign_flag(&flag[i], str[i], one, two);
		i++;
	}
	new = ft_malloc(ft_strlen(str) - count_rems(flag) + 1, ALLOCATE);
	return (copy_ifnot_quote(str, flag, new, flags));
}

#include <dirent.h>
#include <sys/types.h>

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

char	*expand_wildcard(char *pattern, char *is_wild, int i)
{
	int				flag;
	char			*matched;
	DIR				*dir;
	struct dirent	*entry;

	flag = 0;
	while (is_wild[i])
	{
		if (is_wild[i] == IS_WILD)
			flag = 1;
		i++;
	}
	if (!flag)
		return (pattern);
	dir = opendir(".");
	matched = NULL;
	while ((entry = readdir(dir)) != NULL)
	{
		if (match_wildcard(pattern, entry->d_name, is_wild))
		{
			matched = ft_strjoin(matched, entry->d_name, SOUJAOUR);
			matched = ft_strjoin(matched, " ", SOUJAOUR);
		}
	}
	return (matched);
}

char	*expand_cmd(t_chain *cmd, t_argv *args, t_env *env)
{
	char	*join;
	char	*flags;
	char	*temp;
	char	*actual;

	flags = NULL;
	join = expand_str(cmd->content, 0, &flags, env);
	join = remove_quotes(join, flags, 0, 0);
	actual = expand_wildcard(join, store_last(NULL, RETRIEVE), 0);
	while (args)
	{
		join = expand_str(args->content, 0, &flags, env);
		join = remove_quotes(join, flags, 0, 0);
		temp = expand_wildcard(join, store_last(NULL, RETRIEVE), 0);

		actual = ft_strjoin(actual, " ", SOUJAOUR);
		actual = ft_strjoin(actual, temp, SOUJAOUR);

		args = args->next;
	}
	char **arr = custom_split(actual, ' ', SOUJAOUR);
	int i = 0;
	while (arr && arr[i])
	{
		printf("[%s]\n", arr[i]);
		i++;
	}
	return (actual);
}
