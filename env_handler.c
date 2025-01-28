#include "minishell.h"

static t_env *ft_lstlast_env(t_env *env)
{
	t_env	*temp;

	if (!env)
		return (NULL);
	temp = env;
	while (temp->next)
		temp = temp->next;
	return (temp);
}

static void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	if (lst && *lst)
		ft_lstlast_env(*lst)->next = new;
	else
		*lst = new;
}

// Function to check if an env_var already existed
int check_env(t_env *env, char *key)
{
    if (!env)
        return (0);
    while (env)
    {
        if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
            return (1);
        env = env->next;
    }
    return (0);
}

char *expand_env_var(t_env *env, char *exp_env)
{
    while (env)
    {
        if (ft_strncmp(env->key, exp_env, ft_strlen(exp_env)) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

// Create a new env var node
t_env *create_new_env(char *line)
{
    char **splited_line;
    t_env *new_env;
    int i;

    new_env = malloc(sizeof(t_env));
    if (!new_env)
        return (NULL);
    splited_line = ft_split(line, '=');
    if (!splited_line)
        return (NULL);
    new_env->key = ft_strdup(splited_line[0]);
    new_env->value = ft_strdup(splited_line[1]);
    new_env->full = ft_strdup(line);
    new_env->next = NULL;
    i = -1;
    while (splited_line[++i])
        free(splited_line[i]);
    free(splited_line);
    return (new_env);
}



// Function to free env var linked list; Is it useful ?
void free_env(t_env *env)
{
    t_env *free_tmp;
	while (env)
	{
		free_tmp = env;
		env = env->next;
		free(free_tmp);
	}
}

// Function to convert every line of envp as env node
t_env *handle_env(char **envp)
{
    int i = -1;
    t_env *head;
    t_env *new;

    head = NULL;
    while (envp[++i])
    {
        new = create_new_env(envp[i]);
        ft_lstadd_back_env(&head, new);
    }
    return (head);
}

// Function to add a new env var to the env linked list

t_env *get_env_var(t_env *env, char *key)
{
    while (env)
    {
        if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
            return (env);
        env = env->next;
    }
    return (NULL);
}

// Create a new env var received from the user input and add it
// to the env var linked list
// This function need to be optimized and handle memory leaks
void   add_env_var(t_env *env, char *line)
{
    char **splited_line;
    t_env *new_env;
    int i;
    t_env *temp = NULL;
    char *new_full;

    // Make the verification here if an env already exited and just 
    // modified the value if it is else just add it to the linked list
    new_env = malloc(sizeof(t_env));
    if (!new_env)
        return ;
    splited_line = ft_split(line, '=');
    if (!splited_line)
        return ;
    if (splited_line[0][ft_strlen(splited_line[0]) - 1] == '+')
        new_env->key = ft_substr(splited_line[0], 0, ft_strlen(splited_line[0])- 1);
    else
        new_env->key = ft_strdup(splited_line[0]);
    if (check_env(env, new_env->key) && splited_line[0][ft_strlen(splited_line[0]) - 1] == '+')
    {
        temp = get_env_var(env, new_env->key);
        temp->value = ft_strjoin(temp->value, splited_line[1]);
        printf("%s\n", temp->value);
        new_full = ft_strjoin(new_env->key, "=");
        printf("%s\n", new_full);
        temp->full = ft_strjoin(new_full, temp->value);
        printf("%s\n", temp->full);
        free(new_env->key);
        free(new_env);
        free(new_full);
        i = -1;
        while (splited_line[++i])
            free(splited_line[i]);
        free(splited_line);
        return ;
    }
    else if (check_env(env, new_env->key) && splited_line[0][ft_strlen(splited_line[0]) - 1] != '+')
    {
        temp = get_env_var(env, new_env->key);
        temp->value = ft_strdup(splited_line[1]);
        printf("%s\n", temp->value);
        temp->full = ft_strdup(line);
        printf("%s\n", temp->full);
        free(new_env->key);
        free(new_env);
        i = -1;
        while (splited_line[++i])
            free(splited_line[i]);
        free(splited_line);
        return ;
    }
    new_env->value = ft_strdup(splited_line[1]);
    new_env->full = ft_strdup(line);
    new_env->next = NULL;
    i = -1;
    while (splited_line[++i])
        free(splited_line[i]);
    free(splited_line);
    ft_lstadd_back_env(&env, new_env);
}

// Function to create an array using our env var linked list
static int	ft_lstsize_env(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

char **generate_env_var_arr(t_env *env)
{
    int  env_size;
    char **envp;
    int    i;

    env_size = ft_lstsize_env(env);
    envp = malloc((env_size + 1) * sizeof(char *));
    if (!envp)
        return (NULL);
    i = 0;
    while (env && i < env_size)
    {
        envp[i] = ft_strdup(env->full);
        if (!envp[i])
        {
            while (envp[--i])
                free(envp[i]);
            free(envp);
        }
        env = env->next;
        i++;
    }
    envp[i] = NULL;
    return (envp);
}

