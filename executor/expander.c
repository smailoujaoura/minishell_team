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
		*singles = 1;
	else if (c == '\'' && *singles)
		*singles = 0;
	if (c == '"' && !*doubles && !*singles)
		*doubles = 1;
	else if (c == '"' && *doubles)
		*doubles = 0;
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
				len++;
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

char	*expand_cmd(t_chain *cmd, t_argv *args, t_env *env)
{
	// char	**ret;
	char	*first;
	char	*subseq;
	char	*flags;

	flags = NULL;
	first = expand_str(cmd->content, 0, &flags, env);
	while (args)
	{
		first = ft_strjoin(first, " ", SOUJAOUR);
		subseq = expand_str(args->content, 0, &flags, env);
		first = ft_strjoin(first, subseq, SOUJAOUR);
		args = args->next;
	}
	// return (ft_split(first, ' ', SOUJAOUR));
	printf("expanded flg: [%s]\n", flags);
	char **arr = ft_split(first, ' ', SOUJAOUR);
	int j = 0;
	while (arr[j])
	{
		printf("[%s]\n", arr[j]);
		j++;
	}
	return (first);
}
