#include "../../minishell.h"

int	is_redir(t_chain *ptr, int f)
{
	if (!ptr)
		return (0);
	if (f == IN + OR + OUT && (is_redir(ptr, IN) || is_redir(ptr, OUT)))
		return (1);
	if (f == IN && (ptr->type == HEREDOC || ptr->type == REDIR_IN))
		return (1);
	else if (f == OUT && (ptr->type == REDIR_APPEND || ptr->type == REDIR_OUT))
		return (1);
	return (0);
}

void	remove_adjacent_redirs(t_chain *list, t_chain *redirs, int f)
{
	t_chain	*non_redir;
	t_chain	*tmp;

	non_redir = redirs;
	while (non_redir && is_redir(non_redir, IN + OR + OUT))
	{
		if (f)
			non_redir = non_redir->back;
		else
			non_redir = non_redir->next;
	}
	if (f)
	{
		if (non_redir)
			non_redir->next = list;
		list->back = non_redir;
	}
	else
	{
		list->next = non_redir;
		if (non_redir)
			non_redir->back = list; // if non_redir is valid 
	}
	while (is_redir(redirs, IN + OR + OUT))
	{
		tmp = redirs->next;
		free(redirs); // need to free content as well.
		redirs = tmp;
	}
}

t_chain	*create_redirs_chain(t_chain *list)
{
	t_chain	*redirs;
	t_chain	*new;

	redirs = NULL;
	while (list && is_redir(list, IN + OR + OUT))
	{
		list->removable = REMOVE;
		new = ft_calloc(1, sizeof(t_chain));
		ft_memcpy(new, list, sizeof(t_chain));
		new->next = NULL;
		new->back = NULL;
		lstadd_back(&redirs, new);
		list = list->next;
	}
	return (redirs);
}

void	add_files(t_chain *list, t_chain *new)
{
	t_chain	*ptr;
	t_chain	*last;

	ptr = list->blk_f;
	if (ptr && ptr->blk_f)
	{
		last = lstlast(ptr->blk_f);
		last->next = new;
		new->back = last;
	}
	else
		list->blk_f = new;
}

void	create_all_redirs(t_chain *list, t_chain *ptr, int f)
{
	t_chain	*redirs_lvl_one;
	t_chain	*last;

	redirs_lvl_one = create_redirs_chain(ptr);
	while (is_redir(ptr, IN + OR + OUT))
		ptr = ptr->next;
	while (ptr)
	{
		if (ptr->back->type == R_PAREN && is_redir(ptr, IN + OR + OUT) && f - 1 > ptr->depth)
		{
			if (redirs_lvl_one)
			{
				last = lstlast(redirs_lvl_one);
				last->next = create_redirs_chain(ptr);
				last->next->back = last;
			}
			else
				redirs_lvl_one = create_redirs_chain(ptr);
		}
		ptr = ptr->next;
	}
	list->blk_f = redirs_lvl_one;
}

char	*copy_if(char *str, char *s, char *f)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (s[i] == 0)
		{
			f[k] = str[i];
			k++;
		}
		i++;
	}
	f[k] = '\0';
	free(str);
	free(s);
	return (f);
}

int	count_removables(char *s)
{
	int i;
	int	rem;

	i = 0;
	rem = 0;
	while (s[i])
	{
		if (s[i] == 1)
			rem++;
		i++;
	}
	return (rem);
}

char	*remove_occurences(char *str, int i, int singles, int doubles)
{
	char	*s;
	char	*f;

	s = ft_calloc(ft_strlen(str) + 1, 1);
	while (str[i])
	{
		if (str[i] == '"' && singles != 1)
			doubles++;
		if (str[i] == 39 && doubles != 1)
			singles++;
		if (doubles == 2)
			doubles = 0;
		if (singles == 2)
			singles = 0;
		if ((singles != 1 && doubles != 2 && str[i] == '"')
			|| (doubles != 1 && singles != 2 && str[i] == 39))
			s[i] = 1;
		i++;
	}
	f = malloc(ft_strlen(str) - count_removables(s) + 1);
	return (copy_if(str, s, f));
}