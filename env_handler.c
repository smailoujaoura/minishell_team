#include "minishell.h"

// static void free_tab(char **tab)
// {
//     int i;

//     i = -1;
//     while (tab[++i])
//         free(tab[i]);
//     free(tab);
// }

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
static int check_env(t_env *env, char *key)
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
static t_env *create_new_env(const char *line)
{
    char **splited_line;
    t_env *new_env;

    new_env = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
    splited_line = ft_split(line, '=', BKOLANI);
    new_env->key = ft_strdup(splited_line[0], BKOLANI);
    if (splited_line[1])
        new_env->value = ft_strdup(splited_line[1], BKOLANI);
    new_env->full = ft_strdup(line, BKOLANI);
    new_env->next = NULL;
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
    while (env)
    {
        if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
            return (env);
        env = env->next;
    }
    return (NULL);
}

// $l"s"
// $PWD"$var1'$shit'*"
// Function to check if an env var is valid
static int check_env_str(const char *line, char **str_tab)
{
    int i;

    i = -1;
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
            return (1);
        }
    }
    return (0);
}

// Add a new env var without plus op
static void    add_new_env(t_env *env, t_env *new_env, const char *line, char *str)
{
    if (!str)
        new_env->value = ft_strdup("", BKOLANI);
    else
        new_env->value = ft_strdup(str, BKOLANI);  
    new_env->full = ft_strdup(line, BKOLANI);
    new_env->next = NULL;
    ft_lstadd_back_env(&env, new_env);
}

// Add a new env var with plus op
static void    add_new_env_with_plus(t_env *env, t_env *new_env, const char *str)
{
    char *new_full;

    new_full = ft_strjoin(new_env->key, "=", BKOLANI);
    if (!str)
        new_env->full = ft_strdup(new_full, BKOLANI);
    else
    {
        new_env->value = ft_strdup(str, BKOLANI);
        new_env->full = ft_strjoin(new_full, new_env->value, BKOLANI);
    }
    new_env->next = NULL;
    ft_lstadd_back_env(&env, new_env);
}

// Concat or append the new value at the end of the old one; if in the input 
// we found the plus op;
static void    update_env_concat(t_env *env, t_env *new_env, const char *str)
{
    char *new_full;
    t_env *temp;
    char    *value;

    temp = get_env_var(env, new_env->key);
    value = ft_strdup(temp->value, BKOLANI);
    free(temp->value);
    free(temp->full);
    temp->full = NULL;
    temp->value = NULL;
    if  (!str)
    {
        new_full = ft_strjoin(new_env->key, "=", BKOLANI);
        temp->full = ft_strdup(new_full, BKOLANI);
    }
    else
    {
        temp->value = ft_strjoin(value, str, BKOLANI);
        new_full = ft_strjoin(new_env->key, "=", BKOLANI);
        temp->full = ft_strjoin(new_full, temp->value, BKOLANI);
    }
}

// Trunc or overwrite the value of an env var if it exists
static void    update_env_trunc(t_env *env, t_env *new_env, const char *line, const char *str)
{
    t_env *temp = NULL;

    temp = get_env_var(env, new_env->key);
    temp->value = NULL;
    temp->full = NULL;
    if (!str)
        temp->value = ft_strdup("", BKOLANI);
    else
        temp->value = ft_strdup(str, BKOLANI);
    temp->full = ft_strdup(line, BKOLANI);
}

// For each case use the rigth function
static void    process_env_var(t_env *env, t_env *new_env, char **str_tab, const char *line)
{
    if (str_tab[0][ft_strlen(str_tab[0]) - 1] == '+')
        new_env->key = ft_substr(str_tab[0], 0, ft_strlen(str_tab[0])- 1, BKOLANI);
    else
        new_env->key = ft_strdup(str_tab[0], BKOLANI);
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

// Handle export command without args
static void    export_with_no_args(t_env *env)
{
    while (env)
    {
        printf("declare -x %s\n", env->full);
        env = env->next;
    }
}

void check_export_env(t_env *env, char *line)
{
    char **splited_line;
    t_env *new_env;

    if (!ft_strchr(line, '=') && line[ft_strlen(line) -1] == '+')
    {
        printf("minishell: export: `%s': not a valid identifier\n", line);
        return ;
    }
    else if (!ft_strchr(line, '='))
        return ;
    new_env = ft_malloc_bkol(sizeof(t_env), ALLOCATE);
    splited_line = ft_split(line, '=', BKOLANI);
    if (check_env_str(line, splited_line))
    {     
        ft_malloc_bkol(0, DEALLOCATE);
        return ;
    }
    process_env_var(env, new_env, splited_line, line);
}

void   builtin_export(t_env *env, char **argv)
{
    int  i;

    if (!argv)
    {
        export_with_no_args(env);
        return ;
    }
    i = 0;
    while (argv[++i])
        check_export_env(env, argv[i]);
}

// Print env vars
void    builtin_env(t_env *env, char **argv)
{
    if (!env)
        return ;
    if (argv[1])
    {
        printf("env: '%s': No such file or directory\n", argv[1]);
        return ;
    }
    while (env)
    {
        printf("%s\n", env->full);
        env = env->next;
    }
}

// Unset an env var
// Function to free env var linked list; Is it useful ?
static void free_env(t_env *env_to_unset)
{
    free(env_to_unset->key);
    free(env_to_unset->value);
    free(env_to_unset->full);
    free(env_to_unset);
}

static void remove_and_rebuilt(t_env *env, char *arg)
{
    t_env *temp;

    temp = NULL;
    while (env)
    {
        temp = env->next;
        if (ft_strncmp(env->key, arg, ft_strlen(arg)) == 0)
        {
            free_env(env);
            env = temp;
            break ;
        }
        else if (ft_strncmp(temp->key, arg, ft_strlen(arg)) == 0)
        {
            if (temp->next)
                env->next = temp->next;
            else
                env->next = NULL;
            free_env(temp);
            break ;
        }
        env = env->next;
    }
}

void    builtin_unset(t_env *env, char **argv)
{
    int i;

    i = 0;
    if (!argv[1])
        return ;
    while (argv[++i])
    {
        if (!get_env_var(env, argv[i]))
            return ;
        remove_and_rebuilt(env, argv[i]);
    }
}
