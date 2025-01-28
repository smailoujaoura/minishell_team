#ifndef MINISHELL_H
# define MINISHELL_H

#include "./utils/libft/libft.h"

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

# include <signal.h>

// include for readline & history functionality
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
# define SINGLES 1014

// # define L_PAREN "("
// # define R_PAREN ")"
// # define OR "||"
// # define PIPE "|"
// # define AND "&&"
// # define REDIR_APPEND ">>"
// # define REDIR_OUT ">"
// # define HEREDOC "<<"
// # define REDIR_IN "<"
// # define WORD "Word"
// # define DOLLAR "$"
// # define WILDCARD "*"
// # define QUOTES "\""
// # define SINGLES "'"

// # define L_PAREN '('
// # define R_PAREN ')'
// # define OR 1001
// # define PIPE '|'
// # define AND '&'
// # define REDIR_APPEND '+'
// # define REDIR_OUT '>'
// # define HEREDOC 1002
// # define REDIR_IN '<'
// # define WORD 1003
// # define DOLLAR '$'
// # define WILDCARD '*'
// # define QUOTES '"'
// # define SINGLES '\''

// # define L_PAREN 1001
// # define R_PAREN 1002
// # define WORD 1003
// # define PIPE 1005
// # define REDIR_OUT 1005
// # define REDIR_APPEND 1006
// # define REDIR_IN 1007
// # define HEREDOC 1008
// # define AND 1009
// # define OR 10010
// # define WILDCARD 10011
// # define DOLLAR 10012

# define WHITESPACE "\t\n\v\f\r "
# define SYMBOLS "<>|)"

typedef struct s_nodes
{
	int	type;
}	t_nodes;

# define MAXARGS 10
typedef struct s_exec
{
    int		type;
    char	*argv[MAXARGS];
    char	*eargv[MAXARGS];
}	t_exec;

typedef struct s_redir
{
	int		type;
	t_nodes	*cmd;
	char	*file;
	char	*efile;
	int		mode;
	int		fd;
}	t_redir;

typedef struct s_pipe
{
	int		type;
	t_nodes	*left;
	t_nodes	*right;
}	t_pipe;

typedef struct s_heredoc
{
	int		type;
	t_nodes	*left;
	t_nodes	*right;
	t_nodes *cmd;
} t_heredoc;

typedef struct s_and
{
	int		type;
	t_nodes	*left;
	t_nodes	*right;
	char	exit;
} t_and;

typedef struct s_or
{
	int		type;
	t_nodes	*left;
	t_nodes	*right;
	char	exit;
} t_or;

# define VIP 9
# define NAN 0
# define LVL1 1
# define LVL2 2

typedef struct s_chain
{
	int				type;
	char			*content;
	int				dollar;
	int				quote;
	int				lvl;
	struct s_chain	*next;
	struct s_chain	*back;
} t_chain;


// list utilities
t_chain	*lstnew(char *content);
t_chain	*lstlast(t_chain *lst);
void	lstadd_back(t_chain **lst, t_chain *new);
void	move_item(t_chain **src, t_chain **dst, int f);
void	delete_one(t_chain **list);

int	counter(int f);

#endif
