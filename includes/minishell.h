/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 10:22:08 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/17 10:32:52 by soujaour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../utils/libft/libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <stddef.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <stdbool.h>
# include <termios.h>
# include <signal.h>
# include <sys/stat.h>

# define L_PAREN 1001
# define R_PAREN 1002
# define OR 1003
# define PIPE 1004
# define AND 1005
# define REDIR_APPEND 1006
# define REDIR_OUT 1007
# define HEREDOC 1008
# define REDIR_IN 1009
# define WORD 1010
# define DOLLAR 1011
# define QUOTES 1013

# define SUB 1026
# define CMD 1027
# define REMOVE 1015

# define STRT "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_?"
# define MID "01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
# define VAR "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"

# define REMOV 'r'
# define HANDLE 'h'

# define WHITESPACE "\t\n\v\f\r "
# define SYMBOLS "<>|()\"'"

# define IN 0
# define OUT 1

# define RECORD 9327
# define SORT 1232

# define VIP 9
# define NAN 0
# define LVL1 2
# define LVL2 1

# define FROM_VAR 'v'
# define LITERAL 'l'
# define SPLIT 's'

# define IS_WILD 'w'
# define NOT_WILD 'n'

# define REMOVE_QUOTE 'r'
# define STORE 111
# define RETRIEVE 222

# define NOT_QUOTE '!'
# define QUOTE 'q'

# define UPDATE 23
# define CREATE 46
# define INVALID 92

# define SYNTAXERR "Minishell: syntax error near unexpected token"

typedef struct termios	t_term;

// Struct for arguments which is a assigned to a variable in t_chain
typedef struct s_argv
{
	int				type;
	char			*content;
	struct s_argv	*next;
	struct s_argv	*back;
}	t_argv;

// Struct of linked list of tokens 
typedef struct s_chain
{
	int				type;
	char			*content;
	t_argv			*argv;
	struct s_chain	*adj_f;
	char			*file;
	char			*delim;
	int				delim_in_quotes;
	int				lvl;
	int				empty;
	int				removable;
	int				error;
	int				ambiguous;
	struct s_chain	*next;
	struct s_chain	*back;
	int				fd;
}	t_chain;

// Struct for the abstract syntax tree
typedef struct s_ast
{
	int				type;
	int				sub;
	struct s_chain	*data;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

// Struct for list of the environment's variables
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// Struct that will be sent around in function calls
typedef struct s_shell
{
	t_env	*env;
	int		last_exit;
	int		volatile_exit;
	int		export;
	int		flag;
	int		error;
	int		num;
	int		singles;
	int		doubles;
}	t_shell;

// Parsing
void	minishell(t_shell *mini, struct termios *initial);
void	convert_str(char *str, t_chain **list);
void	tokenize_list(t_chain *list);
void	handle_quotes(char **start, char target);
void	handle_redirs(char **start);
t_ast	*parse_line(char *line, t_chain **list, t_shell *mini);
void	prioritize_list(t_chain *list);
void	join_redirs(t_chain *list);
void	join_commands(t_chain *list, t_argv *argv, t_argv *new);
t_chain	*assign_inputs(t_chain *list);
t_chain	*convert_infix(t_chain *infix, t_chain *post, t_chain *ops);
void	remove_if(t_chain *list);
void	delete_any(t_chain *ptr, int i);
char	*remove_occurences(char *str, int i, int singles, int doubles);
t_ast	*build_tree(t_chain *post);
int		store_line(char *new, int flag);
int		is_redir(t_chain *ptr, int f);
void	delete_any(t_chain *ptr, int i);
void	remove_if(t_chain *list);
int		check_paren(t_chain *prev, t_chain *next, int paren);
int		check_redirs(t_chain *prev, t_chain *next);
int		check_logicals(t_chain *prev, t_chain *current, t_chain *next);
int		check_pipe(t_chain *prev, t_chain *current, t_chain *next);
int		check_syntax(t_chain *list, char *line, int l_paren, int r_paren);
t_chain	*special_redir_case(t_chain *list);
void	pre_picker(t_chain *list);
void	post_picker(t_chain *list);
void	lefts_picker(t_chain *list);
t_chain	*create_empty(t_chain *start);

// Utils
void	*ft_malloc(size_t size, int flag);
void	*ft_malloc_bkol(size_t size, int flag);
void	raise_error(char *ptr);
t_chain	*lstnew(char *content);
t_chain	*lstlast(t_chain *lst);
void	lstadd_back(t_chain **lst, t_chain *new);
void	move_item(t_chain **src, t_chain **dst, int f);
void	delete_one(t_chain **list, int i);
void	lstadd_back_arg(t_argv **lst, t_argv *new);
t_argv	*lstlast_arg(t_argv *lst);
t_argv	*lstnew_arg(t_chain *cmd);

// Heredoc
int		open_heredocs(t_chain *list, t_shell *mini);
char	*generate_random_name(void);
int		here_doc(t_chain *data, t_shell *mini);
void	count_heredocs(t_chain *list);
void	strip_heredoc(t_chain *node, char *delim);
void	init_shell(char **envp, t_shell *mini);

// Env functions and their utils
t_env	*handle_env(char **envp, int i, char *cwd);
t_env	*get_env_var(t_env *env, const char *key);
char	*expand_env_var(t_env *env, char *exp_env);
t_env	*get_env_var(t_env *env, const char *key);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
int		check_env(t_env *env, char *key);
char	*expand_env_var(t_env *env, char *exp_env);
char	**make_env(char **envp);
int		ft_lstsize_env(t_env *env);

// Builtins
char	**splitter(char *str, int *action);
char	*store_pwd(char *store_it, int flag);
int		is_valid_key(char *key);
void	print_keys_values(t_env *env);
void	builtin_pwd(t_shell *mini);
void	builtin_echo(char **argv, int *status);
void	builtin_exit(char **argv, int *status);
void	builtin_env(t_env *env);
void	builtin_cd(t_env *env, char **argv, int *status);
int		builtin_export(t_env *env, char **argv, int flag);
void	builtin_unset(t_shell *mini, char **argv);
void	ft_update_oldpwd(t_env *env, char *path);
void	ft_update_pwd(t_env *env, char *path);

// Expanding
char	**expand_cmd(t_chain *cmd, t_argv *args, t_shell *mini);
void	expand_redirs(t_chain *ptr, t_shell *mini);
char	*ultimate(char *str, char *flags, int one, int two);
char	*remove_flags(char *flags, char **actual, char *str);
void	expand_heredoc(t_chain *ptr, t_shell *mini, char *new, int source_fd);
char	**ultimate_split(char *str, char *flags, char sep, t_list *list);
char	*wild_shell(char *sources, char *quotes, char *str);
int		is_var(char current, char next, char *set);
char	*get_value(char *str, int *i, t_shell *mini, int type);
char	*handle_var_values(char *value, char *new, char **flags, t_shell *mini);
char	*just_copy(char *str, int *i, int *singles, int *doubles);
void	construct_flags(char *str, char flag, char **flags);
void	expand_wildcard(t_list **list, char *pattern, char *is_wild);
char	*get_value_wrapper(char *var, t_env *env);
int		find_split(char *flags, int i);
char	**ft_split_custom(char const *s, char *set, int f);
void	check_export(t_shell *mini, char *new);

// Signals
void	setup_signals(int action);
void	handle_interrupt(void);
void	first_handler(int signum);
void	second_handler(int signum);
void	third_handler(int signum);
void	statically_stored_shell(t_shell *mini, int action);
void	return_status(int signum);

// Executor
int		open_and_assign(t_chain *redirs);
void	executor(t_ast *tree, t_shell *mini);
void	run_and(t_ast *tree, t_shell *mini);
void	run_or(t_ast *tree, t_shell *mini);
int		is_empty_command(char **argv, t_shell *mini);
int		assign_fds_builtins(t_ast *tree, char *cmd, int action);
void	external_cmd(t_ast *tree, char **argv, char **envp, t_shell *mini);
char	*construct_cmd_path(char **argv, t_env *envp, int i);
char	**generate_env_tab(t_env *envp);
int		check_builtin(const char *cmd);
void	execute_builtin(t_ast *tree, char **argv, t_shell *mini);
void	pipe_child(t_ast *tree, t_shell *mini, int *pipe_fd, int flag);
void	external_cmd(t_ast *tree, char **argv, char **envp, t_shell *mini);
int		ft_dup2(int old, int new);
int		ft_fork(void);
int		ft_dup(int old);
int		ft_dup2(int old, int new);
int		ft_pipe(int *pair);
void	ft_exit(int status);

#endif