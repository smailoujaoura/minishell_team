#include "../minishell.h"

// we need stack behaviour sometimes so we specify that with f(flag) = 1,
// otherwise f = 0, meaning adding the new node to the back of the list
void	move_item(t_chain **src, t_chain **dst, int f) 
{
	t_chain	*tmp;

	if (!dst || !src || !(*src))
		return ;
	if (f == 0)
	{
		// save node to be moved in tmp; and update src to point to subsequent one
		tmp = *src;
		*src = tmp->next;
		if (*src)
			(*src)->back = NULL;
		tmp->next = NULL;
		// add the tmp node to dst
		lstadd_back(dst, tmp);
		return ;
	}
	tmp = *src;
	*src = tmp->next;
	if (*src) // this is for protecting when the stack is empty in order not to segfault derefrencing a NULL
		(*src)->back = NULL;
	tmp->back = NULL;
	tmp->next = *dst;
	*dst = tmp;
}

void	delete_one(t_chain **list, int i)
{
	t_chain	*tmp;

	if (!list || !(*list))
		return ;
	// point tmp to the node to be removed and then update the head to point to the next one
	tmp = *list;
	*list = tmp->next;
	if (*list)
		(*list)->back = NULL;
	(void)i;
	// remove the content in the node and the node itself
	// if (i)
	// 	free(tmp->content);
	// free(tmp);
}

t_chain	*lstnew(char *content)
{
	t_chain	*new;

	new = ft_malloc(sizeof(t_chain), ALLOCATE);
	new->content = content;
	new->type = -1;
	new->lvl = -1;
	new->depth = 0;
	new->empty = 0;
	new->next = NULL;
	new->back = NULL;
	new->delim = NULL;
	new->file = NULL;
	new->adj_f = NULL;
	new->blk_f = NULL;
	new->argv = NULL;
	new->pipe = NULL;
	new->removable = 0;
	new->wildcard = 0;
	new->dollar = 0;
	new->delim_in_quotes = 0;
	new->ambiguous = 0;
	new->error = 0;
	new->fd = -2;
	return (new);
}

t_chain	*lstlast(t_chain *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

void	lstadd_back(t_chain **lst, t_chain *new)
{
	t_chain	*last;

	if (!lst || !new)
		return ;
	if (*lst)
	{
		last = lstlast(*lst);
		last->next = new;
		new->back = last;
	}
	else
	{
		new->back = NULL;
		*lst = new;
	}
}

t_argv	*lstnew_arg(t_chain *cmd)
{
	t_argv	*new;

	new = ft_malloc(sizeof(t_argv), ALLOCATE);
	new->back = NULL;
	new->next = NULL;
	new->type = -1;
	new->wildcard = cmd->wildcard;
	new->dollar = cmd->dollar;
	new->content = ft_strdup(cmd->content, SOUJAOUR); // should it be strdup or not? s
	return (new);
}

t_argv	*lstlast_arg(t_argv *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

void	lstadd_back_arg(t_argv **lst, t_argv *new)
{
	t_argv	*last;

	if (!lst || !new)
		return ;
	if (*lst)
	{
		last = lstlast_arg(*lst);
		last->next = new;
		new->back = last;
	}
	else
	{
		new->back = NULL;
		*lst = new;
	}
}

t_ast	*free_list(t_chain *list)
{
	// return
	(void)list;
	return (NULL);
}