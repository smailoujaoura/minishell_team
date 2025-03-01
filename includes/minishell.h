/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soujaour <soujaour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 10:22:08 by soujaour          #+#    #+#             */
/*   Updated: 2025/03/01 13:08:17 by soujaour         ###   ########.fr       */
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
# include <signal.h>
# include <errno.h>
# include <stddef.h>
# include <dirent.h>
# include <sys/wait.h>
# include <dirent.h>
# include <sys/types.h>
# include <stdbool.h>

// General Macros
# define MEMORY_ERROR "Memory Error!"

// Related to Expanding
# define RECORD 9327
# define SORT 1232

# define MID "01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
# define STRT "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_?"

# define REMOV 'r'
# define HANDLE 'h'

// Macros for token recognization
# define WHITESPACE "\t\n\v\f\r "
# define SYMBOLS "<>|()\"'"

# define SINGLES '\''
# define DOUBLES '"'

// Easy pipes ends remembering
# define READ_END 0
# define WRITE_END 1

// Tokenizer types: all except SUB/CMD; AST types: SUB, OR, AND, CMD, PIPE
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
# define WILDCARD 1012
# define QUOTES 1013
# define SUB 1016
# define CMD 1027
# define WILDCARDS 1020

// Identify redir mode
# define IN 0
# define OUT 1

// Assigning this type for some nodes to be removed later on, like redirs...
# define REMOVE 1015

/*
Macros for operators and terms priority: 
	VIP for all commands: ls, echo, grep, ./a.out, ... 
	LVL1 for pipes, 
	LVL2 for logicals "&&" and "||"
	NAN for parenthesis, which have no priority
this to apply the Shunting Yard algorithm
*/
# define VIP 9
# define NAN 0
# define LVL1 2
# define LVL2 1

// Macros for expanding
# define FROM_VAR 'v'
# define LITERAL 'l'
# define IGNORE 'i'
# define SPLIT 's'

# define IS_WILD 'w'
# define NOT_WILD 'n'

# define REMOVE_QUOTE 'r'
# define STORE 111
# define RETRIEVE 222

# define SEPERATORS " \t"

# define NOT_QUOTE '!'
# define QUOTE 'q'

// Struct for arguments which is a assigned to a variable in t_chain
typedef struct s_argv
{
	int				type;
	char			*content;
	struct s_argv	*next;
	struct s_argv	*back;
}	t_argv;

/*
	Each token takes a node. Then, the list is compacted to only these types:
	
	1) first:
		- Redirections absorb their filesnames/delimiters

	2) second:
		- Command, this node absorbs nodes of args and redirs adjacent to it
		- Pipe 
		- && 
		- ||
		- (
		- )
		Bad design but works.
*/
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
	struct s_chain	*data;
	char			**argv;
	int				out_fd;
	int				in_fd;
	struct s_ast	*left;
	struct s_ast	*right;
	int				exit_status;
}	t_ast;

// Struct for list of the environment's variables
typedef struct s_env
{
	char			*key;
	char			*value;
	char			*full;
	struct s_env	*next;
}	t_env;

// Struct that will be sent around in function calls
typedef struct s_shell
{
	t_env	*env;
	int		last_exit;
	bool	should_execute;
}	t_shell;

// General
void	panic_exit(char *ptr, int place);

// Garbage Collectors
void	*ft_malloc(size_t size, int flag);
void	*ft_malloc_bkol(size_t size, int flag);
void	garbage_collector(t_list *allocs, void *one, void *two);

// Lexing
void	convert_str(char *str, t_chain **list);
void	tokenize_list(t_chain *list);
void	handle_quotes(char **start, char target, char opposite);
void	handle_redirs(char **start);

// Parser
t_ast	*parse_line(char *line, t_chain **list, int *num);
void	prioritize_list(t_chain *list);
void	join_redirs(t_chain *list);
void	join_commands(t_chain *list, t_argv *argv, t_argv *new);
t_chain	*assign_inputs(t_chain *list, t_chain *ptr);
void	pick_left_redirs(t_chain *list);
t_chain	*convert_infix(t_chain *infix, t_chain *post, t_chain *ops);
void	assign_block_redirs(t_chain *list);
void	assign_adjacent_redirs(t_chain *list, t_chain *ptr);
t_chain	*assign_inputs_edges(t_chain *list);
void	remove_if(t_chain *list);
void	delete_any(t_chain *ptr, int i);
char	*remove_occurences(char *str, int i, int singles, int doubles);
void	create_all_redirs(t_chain *list, t_chain *ptr, int f);
void	add_files(t_chain *list, t_chain *new);
void	remove_adjacent_redirs(t_chain *list, t_chain *redirs, int f);
t_ast	*build_tree(t_chain *post);
void	store_line(char *new, int flag);
int		is_redir(t_chain *ptr, int f);
void	delete_any(t_chain *ptr, int i);
void	remove_if(t_chain *list);
int		check_paren(t_chain *prev, t_chain *next, int paren);
int		check_redirs(t_chain *prev, t_chain *next);
int		check_logicals(t_chain *prev, t_chain *next);
int		check_pipe(t_chain *prev, t_chain *next);
int		check_syntax(t_chain *list, char *line, int l_paren, int r_paren);

# define SYNTAXERR "Minishell: syntax error near unexpected token"
# define ERR "Minishell: syntax error near unexpected token '%s'\n"

// List utils for parser
t_chain	*lstnew(char *content);
t_chain	*lstlast(t_chain *lst);
void	lstadd_back(t_chain **lst, t_chain *new);
void	move_item(t_chain **src, t_chain **dst, int f);
void	delete_one(t_chain **list, int i);
void	lstadd_back_arg(t_argv **lst, t_argv *new);
t_argv	*lstlast_arg(t_argv *lst);
t_argv	*lstnew_arg(t_chain *cmd);

// Redirections
t_chain	*assign_inputs_edges(t_chain *list);
t_chain	*create_redirs_chain(t_chain *list);
t_ast	*free_list(t_chain *list);

// Heredoc
char	*generate_random_name(void);
void	here_doc(t_chain *data, int num);

// Env functions and their utils
t_env	*handle_env(char **envp);
t_env	*get_env_var(t_env *env, const char *key);
char	*expand_env_var(t_env *env, char *exp_env);
void	add_new_env(t_env *env, t_env *new_env, const char *line, char *str);
void	add_new_env_with_plus(t_env *env, t_env *new_env, const char *str);
void	update_env_trunc(t_env *env, t_env *new_env,
			const char *line, const char *str);
void	update_env_concat(t_env *env, t_env *new_env, const char *str);
t_env	*handle_env(char **envp);
t_env	*get_env_var(t_env *env, const char *key);
int		check_env_str(const char *line, char **str_tab);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
int		check_env(t_env *env, char *key);
char	*expand_env_var(t_env *env, char *exp_env);

// Builtins
void	builtin_pwd(t_shell *mini);
void	builtin_echo(char **argv, int *status);
void	builtin_exit(char **argv, int *status);
void	builtin_env(t_env *env, char **argv);
void	builtin_cd(t_env *env, char **argv, int *status);
void	builtin_export(t_env *env, char **argv, int flag);
void	builtin_unset(t_env *env, char **argv);

// Expanding
char	**expand_cmd(t_chain *cmd, t_argv *args, t_shell *mini);
void	expand_redirs(t_chain *ptr, t_shell *mini);
char	*ultimate(char *str, char *flags, int one, int two);
char	*remove_flags(char *flags, char **actual, char *str);
void	expand_heredoc(t_chain *ptr, t_shell *mini, char *new, int source_fd);
char	**ultimate_split(char *str, char *flags, char sep, t_list *list);
char	*wild_shell(char *sources, char *quotes, char *str);
int		is_var(char current, char next, char *set);
char	*get_value(char *str, int *i, t_shell *mini);
char	*handle_var_values(char *value, char *new, char **flags, int two);
char	*just_copy(char *str, int *i, int *singles, int *doubles);
void	construct_flags(char *str, char flag, char **flags);
void	expand_wildcard(t_list **list, char *pattern, char *is_wild);
char	*get_value_wrapper(char *var, t_env *env);

// Executor
void	executor(t_ast *tree, t_shell *mini);
void	assign_fds_builtins(t_ast *tree, int action);
void	assign_fds(t_ast *tree);
void	buildin_excutor(t_ast *tree, char **argv, t_shell *mini);
void	external_cmd(t_ast *tree, char **argv, char **envp, t_shell *mini);
char	*construct_cmd_path(char **argv, t_env *envp);
char	**generate_env_tab(t_env *envp);
int		check_buildin(const char *cmd);
bool	create_adj_files(t_chain *adj);
void	buildin_excutor(t_ast *tree, char **argv, t_shell *mini);
void	fds_dup(t_ast *tree);
void	pipe_child(t_ast *tree, t_shell *mini, int *pipe_fd, int flag);
void	external_cmd(t_ast *tree, char **argv, char **envp, t_shell *mini);

#endif
