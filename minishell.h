#ifndef MINISHELL_H
# define MINISHELL_H

#include "./utils/libft/libft.h"

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <stddef.h>
# include "dirent.h"
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
# define REMOVE 1015

# define WILDCARDS 1020

# define MEMORY_ERROR "Memory Error!"
void	panic_exit(char *s);

# define WHITESPACE "\t\n\v\f\r "
# define SYMBOLS "<>|()\"'"

# define SINGLES '\''
# define DOUBLES '"'

# define VIP 9
# define NAN 0
# define LVL1 2
# define LVL2 1

# define IN 0
# define OUT 1

// execute(expand(argv->content), )
typedef struct s_exp
{
	char			*val;
	struct s_exp	*next;
} t_exp;

typedef struct s_argv
{
	int				type;
	char			*content;
	t_exp			*exp;
	int				wildcard;
	int				dollar;
	struct s_argv	*next;
	struct s_argv	*back;
}	t_argv;

typedef struct s_chain
{
	int				type;
	int				depth;
	int				lvl;
	int				empty;
	char			*content;
	int				wildcard;
	int				dollar;
	t_argv			*argv;
	t_exp			*exp;
	char			*file;
	char			*delim;
	int				delim_in_quotes;
	struct s_chain	*next;
	struct s_chain	*back;
	struct s_chain	*adj_f;
	struct s_chain	*blk_f;
	int				removable;
	int				ambiguous;
}	t_chain;

# define ROOT 0
# define LEFT -1
# define RIGHT 1


// typedef struct s_status
// {
// 	int	status;
// }	t_status;

// Abstract Syntax Tree to represent the the parsed line
	// char			*cmd;
	// char			**args;
typedef struct s_ast
{
	int				type;
	struct s_chain	*data;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*parent;
	int				side;
	int				exit_status;
	int				*pipe;
}	t_ast;

typedef struct s_env
{
    char    *key;
    char    *value;
    char    *full;
    struct s_env *next;
} t_env;

// Garbage Collector
void	*ft_malloc(size_t size, int flag);
void	garbage_collector(t_list *allocs, void *one, void *two);
void	*ft_malloc_bkol(size_t size, int flag);

void	print_with_files(t_chain *ptr);

// Lexing
t_chain	*convert_str(char *str);
void	tokenize_list(t_chain *list);

void	find_type(t_chain *list);
void	handle_quotes(char **start, char target, char opposite);

// PARSER
void	prioritize_list(t_chain *list);
void	assign_depth(t_chain *list);
void	strip_words(t_chain *list);
t_chain	*join_redirs(t_chain *list);
t_chain	*join_commands(t_chain *list);
t_chain	*assign_inputs(t_chain *list, t_chain *ptr);
void	pick_left_redirs(t_chain *list);
t_chain	*convert_infix(t_chain *infix);
void	handle_redirs(char **start);
int	is_redir(t_chain *ptr, int f);
void	assign_block_redirs(t_chain *list);
void	assign_adjacent_redirs(t_chain *list, t_chain *ptr);
t_chain	*assign_inputs_edges(t_chain *list);
void	remove_if(t_chain *list);
void	delete_any(t_chain *ptr, int i);
char	*remove_occurences(char *str, int i, int singles, int doubles);
void	create_all_redirs(t_chain *list, t_chain *ptr, int f);
void	add_files(t_chain *list, t_chain *new);
void	remove_adjacent_redirs(t_chain *list, t_chain *redirs, int f);


// list utilities
t_chain	*lstnew(char *content);
t_chain	*lstlast(t_chain *lst);
void	lstadd_back(t_chain **lst, t_chain *new);
void	move_item(t_chain **src, t_chain **dst, int f);
void	delete_one(t_chain **list, int i);
void	lstadd_back_arg(t_argv **lst, t_argv *new);
t_argv	*lstlast_arg(t_argv *lst);
t_argv	*lstnew_arg(t_chain *cmd);

// Syntax Validator
int	check_syntax(t_chain *list, char *line, int l_paren, int r_paren);

// redir utils
int	is_redir(t_chain *ptr, int f);
t_chain	*assign_inputs_edges(t_chain *list);
t_chain	*create_redirs_chain(t_chain *list);
t_ast	*free_list(t_chain *list);



// convert Post to AST
t_ast	*build_tree(t_chain *post);













//env
t_env	*handle_env(const char **envp);
t_env *get_env_var(t_env *env, const char *key);
char *expand_env_var(t_env *env, char *exp_env);

// Builtins
void    pwd(t_chain *data);
void    cd(t_env *env, t_chain *data);
void    echo(t_chain *data);
void    mini_exit(t_chain *data);
void	mini_env(t_env *env, t_chain *data);
void   	export(t_env *head, t_chain *data);
void    unset(t_env *env, t_chain *data);

// here_doc
void    here_doc(t_chain *data);

// Execution
void	wild_card_handler(t_ast *node, const char **envp);

// Builtins utils
// int		builtins_redir_fd(t_chain *file);
// void    builtins_redir(t_chain *redir_file, char *buff, int create_only);
// void    create_files_only(t_chain *data);
// void    redir_output(t_chain *data, char *output);


// EXPANDING
char	**expand_cmd(t_chain *cmd, t_argv *args, t_env *env);
void	executor(t_ast *tree, t_env *env);
void	expand_redirs(t_chain *adj, t_chain *blk, t_env *env);


char	*get_value(char *var, t_env *env);

#endif
