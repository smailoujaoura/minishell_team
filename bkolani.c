#include "minishell.h"

void	loop_minishell(t_env *env);

void	bkolani(char *argv[], char *envp[])
{
	(void)argv;
	(void)envp;
	(void)argv;
	(void)envp;
	t_env *env;

	env = handle_env(envp);
	t_chain *adj1 = malloc(sizeof(t_chain));
	t_chain *adj2 = malloc(sizeof(t_chain));
	t_chain *adj3 = malloc(sizeof(t_chain));
	t_chain *data =  malloc(sizeof(t_chain));
	adj1->file = "adj1";
	adj1->type = REDIR_OUT;
	adj1->next = adj2;
	adj2->file = "adj2";
	adj2->type = REDIR_OUT;
	adj2->next = adj3;
	adj3->file = "adj3";
	adj3->type = REDIR_APPEND;
	adj3->next = NULL;
	t_chain *blk1 = malloc(sizeof(t_chain));
	t_chain *blk2 = malloc(sizeof(t_chain));
	t_chain *blk3 = malloc(sizeof(t_chain));
	blk1->file = "blk1";
	blk1->type = REDIR_OUT;
	blk1->next = blk2;
	blk2->file = "blk2";
	blk2->type = REDIR_OUT;
	blk2->next = blk3;
	blk3->file = "blk3";
	blk3->type = REDIR_APPEND;
	blk3->next = NULL;
	data->adj_f = adj1;
	data->blk_f = blk1;
	data->argv = NULL;
	data->next = NULL;
	// pwd(data);
	// print_env_vars(env);
	t_argv *args1 = malloc(sizeof(t_argv));
	t_argv *args2 = malloc(sizeof(t_argv));
	t_argv *args3 = malloc(sizeof(t_argv));
	args1->content = "-nnnnnnnnnnnnnnnnnnnn";
	args1->next = args2;
	args2->content = "-nnnnnnnnnnnnnn";
	args2->next = args3;
	args3->content = "Hello, World!";
	args3->next = NULL;
	data->argv = args3;
	data->adj_f = adj1;
	data->blk_f = blk1;
	data->next = NULL;
	echo(data);
	// mini_exit(echo_data);
	// cd(env, data);
	// unset(env, data);
	(void)env;
}
