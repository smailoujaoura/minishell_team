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


extern int i;

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


# define WHITESPACE "\t\n\v\f\r "
# define SYMBOLS "<>|()"

# define SINGLES '\''
# define DOUBLES '"'

# define VIP 9
# define NAN 0
# define LVL1 2
# define LVL2 1

# define IN 0
# define OUT 1

typedef struct s_argv
{
	int				type;
	char			*content;
	int				wildcard;
	int				dollar;
	struct s_argv	*next;
	struct s_argv	*back;
}	t_argv;

typedef struct s_chain
{
	int				type;
	char			*content;
	int				depth;
	int				lvl;
	t_argv			*argv;
	char			*file;
	char			*delim;
	int				delim_in_quotes;
	struct s_chain	*next;
	struct s_chain	*back;
	struct s_chain	*adj_f;
	struct s_chain	*blk_f;
	int				empty;
	int				removable;
	int				wildcard;
	int				dollar;
}	t_chain;


// Abstract Syntax Tree to represent the the parsed line
typedef struct s_ast
{
	int				type;
	struct s_chain	*data;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_env
{
    char    *key;
    char    *value;
    char    *full;
    struct s_env *next;
} t_env;


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
t_env	*handle_env(char **envp);
void print_env_vars(t_env *env);
void   export_env_var(t_env *head, t_argv *args);
void export_with_no_args(t_env *env);
void    unset_env_var(t_env *env, t_argv *args);
t_env *get_env_var(t_env *env, const char *key);

// Builtins
void    pwd(void);
void    cd(t_env *env, t_argv *argv);
void    echo(t_argv *argv);
void    mini_exit(t_argv *argv);

#endif
