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

t_env *get_env_var(t_env *env, const char *key)
{
    printf("Key: %s\n", key);
    while (env)
    {
        if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
        {
            printf("Get env var: %s\n", env->full);
            return (env);
        }
        env = env->next;
    }
    return (NULL);
}

// Function to check if an env var is valid
int    check_env_str(const char *line, char **str_tab)
{
    int i;
    int j;

    i = -1;
    if (!str_tab[0])
        return (1);
    while (str_tab[0][++i])
    {
        if (((str_tab[0][0] >= '0' && str_tab[0][0] <= '9')
            && str_tab[0][ft_strlen(str_tab[0]) - 1] )
            || (!(str_tab[0][i] >= 'a' && str_tab[0][i] <= 'z')
            && !(str_tab[0][i] >= 'A' && str_tab[0][i] <= 'Z')
            && !(str_tab[0][i] >= '0' && str_tab[0][i] <= '9')
            && str_tab[0][i] != '_' && !str_tab[1]))
        {
            printf("Minishell: export: `%s': not a valid identifier\n", line);
            j = -1;
            while (str_tab[++j])
                free(str_tab[j]);
            free(str_tab);
            return (1);
        }
    }
    return (0);
}

// Add a new env var without plus op
void    add_new_env(t_env *env, t_env *new_env, const char *line, char *str)
{
    if (!str)
        new_env->value = ft_strdup("");
    else
        new_env->value = ft_strdup(str);  
    new_env->full = ft_strdup(line);
    new_env->next = NULL;
    ft_lstadd_back_env(&env, new_env);
}

// Add a new env var with plus op
void    add_new_env_with_plus(t_env *env, t_env *new_env, const char *str)
{
    char *new_full;

    new_full = ft_strjoin(new_env->key, "=");
    if (!str)
        new_env->full = ft_strdup(new_full);
    else
    {
        new_env->value = ft_strdup(str);
        new_env->full = ft_strjoin(new_full, new_env->value);
    }
    new_env->next = NULL;
    free(new_full);
    ft_lstadd_back_env(&env, new_env);
}

// Concat or append the new value at the end of the old one; if in the input 
// we found the plus op;
void    update_env_concat(t_env *env, t_env *new_env, const char *str)
{
    char *new_full;
    t_env *temp;
    char    *value;

    temp = get_env_var(env, new_env->key);
    value = ft_strdup(temp->value);
    free(temp->value);
    free(temp->full);
    temp->full = NULL;
    temp->value = NULL;
    if  (!str)
    {
        new_full = ft_strjoin(new_env->key, "=");
        temp->full = ft_strdup(new_full);
    }
    else
    {
        temp->value = ft_strjoin(value, str);
        new_full = ft_strjoin(new_env->key, "=");
        temp->full = ft_strjoin(new_full, temp->value);
    }
    free(new_env->key);
    free(new_env);
    free(value);
    free(new_full);
}

// Trunc or overwrite the value of an env var if it exists
void    update_env_trunc(t_env *env, t_env *new_env, const char *line, const char *str)
{
    t_env *temp = NULL;

    temp = get_env_var(env, new_env->key);
    free(temp->value);
    free(temp->full);
    temp->value = NULL;
    temp->full = NULL;
    if (!str)
        temp->value = ft_strdup("");
    else
        temp->value = ft_strdup(str);
    temp->full = ft_strdup(line);
    free(new_env->key);
    free(new_env);
}

// For each case use the rigth function
void    process_env_var(t_env *env, t_env *new_env, char **str_tab, const char *line)
{
    if (str_tab[0][ft_strlen(str_tab[0]) - 1] == '+')
        new_env->key = ft_substr(str_tab[0], 0, ft_strlen(str_tab[0])- 1);
    else
        new_env->key = ft_strdup(str_tab[0]);
    if (check_env(env, new_env->key) && str_tab[0][ft_strlen(str_tab[0]) - 1] == '+')
        update_env_concat(env, new_env, str_tab[1]);
    else if (check_env(env, new_env->key) && str_tab[0][ft_strlen(str_tab[0]) - 1] != '+')
        update_env_trunc(env, new_env, line, str_tab[1]);
    else if (!check_env(env, new_env->key) && str_tab[0][ft_strlen(str_tab[0]) - 1] == '+')
        add_new_env_with_plus(env, new_env, str_tab[1]);
    else
        add_new_env(env, new_env, line, str_tab[1]);
}

// The main function to add an env var
// Create a new env var received from the user input and add it
// to the env var linked list
void   export_env_var(t_env *env, t_argv *args)
{
    char **splited_line;
    t_env *new_env;
    int i;
    char *line;

    while (args)
    {
        line = args->content;
        if (!ft_strchr(line, '=') && line[ft_strlen(line) -1] == '+')
        {
            printf("export: not valid in this context: %s\n", line);
            return ;
        }
        else if (!ft_strchr(line, '='))
            return ;
        new_env = malloc(sizeof(t_env));
        if (!new_env)
            return ;
        splited_line = ft_split(line, '=');
        if (!splited_line)
            return ;
        if (check_env_str(line, splited_line))
        {     
            free(new_env);
            return ;
        }
        process_env_var(env, new_env, splited_line, line);
        args = args->next;
        i = -1;
        while (splited_line[++i])
            free(splited_line[i]);
        free(splited_line);
    }
}

// Print env vars
void    print_env_vars(t_env *env)
{
    while (env)
    {
        printf("%s\n", env->full);
        env = env->next;
    }
}

// Handle export command without args
void    export_with_no_args(t_env *env)
{
    while (env)
    {
        printf("declare -x %s\n", env->full);
        env = env->next;
    }
}

// Unset an env var
// Function to free env var linked list; Is it useful ?
void free_env(t_env *env_to_unset)
{
    free(env_to_unset->key);
    free(env_to_unset->value);
    free(env_to_unset->full);
    free(env_to_unset);
}

void    unset_env_var(t_env *env, t_argv *args)
{
    int i;
    t_env *temp;
    t_env *env_var_to_unset;
    t_argv *args_temp;

    i = 0;
    args_temp = args;
    while (args_temp)
    {
        if ((!(args_temp->content[0] >= 'a' && args_temp->content[1] <= 'z')
            && !(args_temp->content[0] >= 'A' && args_temp->content[0] <= 'Z')
            && args_temp->content[0] != '_')
            || (!(args_temp->content[ft_strlen(args_temp->content) - 1] >= 'a' && args_temp->content[ft_strlen(args_temp->content) - 1] <= 'z')
            && !(args_temp->content[ft_strlen(args_temp->content) - 1] >= 'A' && args_temp->content[ft_strlen(args_temp->content) - 1] <= 'Z')
            && args_temp->content[ft_strlen(args_temp->content) - 1] != '_'
            && !(args_temp->content[ft_strlen(args_temp->content) - 1] >= '0' && args_temp->content[ft_strlen(args_temp->content) - 1] <= '9'))
            || (!(args_temp->content[i] >= 'a' && args_temp->content[i] <= 'z')
            && !(args_temp->content[i] >= 'A' && args_temp->content[i] <= 'Z')
            && args_temp->content[i] != '_'
            && !(args_temp->content[i] >= '0' && args_temp->content[i] <= '9')))
        {
            printf("unset: %s: invalid parameter name\n", args_temp->content);
            return ;
        }
        args_temp = args_temp->next;
        i++;
    }
    args_temp = args;
    i = 0;
    while (args_temp)
    {
        if ((!(args_temp->content[0] >= 'a' && args_temp->content[0] <= 'z')
            && !(args_temp->content[0] >= 'A' && args_temp->content[0] <= 'Z')
            && args_temp->content[0] != '_')
            || (!(args_temp->content[ft_strlen(args_temp->content) - 1] >= 'a' && args_temp->content[ft_strlen(args_temp->content) - 1] <= 'z')
            && !(args_temp->content[ft_strlen(args_temp->content) - 1] >= 'A' && args_temp->content[ft_strlen(args_temp->content) - 1] <= 'Z')
            && args_temp->content[ft_strlen(args_temp->content) - 1] != '_'
            && !(args_temp->content[ft_strlen(args_temp->content) - 1] >= '0' && args_temp->content[ft_strlen(args_temp->content) - 1] <= '9'))
            || (!(args_temp->content[i] >= 'a' && args_temp->content[i] <= 'z')
            && !(args_temp->content[i] >= 'A' && args_temp->content[i] <= 'Z')
            && args_temp->content[i] != '_'
            && !(args_temp->content[i] >= '0' && args_temp->content[i] <= '9')))
        {
            printf("unset: %s: invalid parameter name\n", args_temp->content);
            return ;
        }
        args_temp = args_temp->next;
        i++;
    }
    temp = NULL;
    env_var_to_unset = NULL;
    while (env && args)
    {
        env_var_to_unset = get_env_var(env, args->content);
        printf("Ici: %s\n", args->content);
        if (ft_strncmp(env->next->key, env_var_to_unset->key, ft_strlen(args->content)) == 0)
        {
            temp = env_var_to_unset->next;
            if (temp)
                env->next = temp;
            else if (!temp)
                env->next = NULL;
            break ;
        }
        // free(env_var_to_unset->value);
        // free(env_var_to_unset->full);
        // free(env_var_to_unset->key);
        // free(env_var_to_unset);
        env = env->next;
        args = args->next;
    }
}