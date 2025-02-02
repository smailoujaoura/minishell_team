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

# define WHITESPACE "\t\n\v\f\r "
# define SYMBOLS "<>|)"

# define VIP 9
# define NAN 0
# define LVL1 2
# define LVL2 1

# define IN 0
# define OUT 1

// Linked list of tokens and their data
typedef struct s_chain
{
	int				type;
	char			*content;
	int				dollar;
	int				quote;
	int				depth;
	int				lvl;
	int				argc;
	char			**argv;
	char			*file;
	char			*delim;
	struct s_chain	*next;
	struct s_chain	*back;
	int				set;
	struct s_chain	*adj_f;
	struct s_chain	*blk_f;
}	t_chain;

// Abstract Syntax Tree to represent the the parsed line
typedef struct s_ast
{
	int	type;
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

// list utilities
t_chain	*lstnew(char *content);
t_chain	*lstlast(t_chain *lst);
void	lstadd_back(t_chain **lst, t_chain *new);
void	move_item(t_chain **src, t_chain **dst, int f);
void	delete_one(t_chain **list, int i);

//env
t_env	*handle_env(char **envp);
char **generate_env_var_arr(t_env *env);
void   add_env_var(t_env *head, char *line);

t_chain	*assign_inputs_edges(t_chain *list);
t_chain	*create_redirs_chain(t_chain *list);

// convert Post to AST
t_ast	*build_tree(t_chain *post);

#endif
