#include "minishell.h"

void	loop_minishell(void);

void	bkolani(char *argv[], char *envp[])
{
	(void)argv;
	(void)envp;
	(void)argv;
	(void)envp;
	t_env *env;
	// t_argv *args_exp = NULL;
	// t_argv *args_unset = NULL;


	// initalize structures and envirnoment
	env = handle_env(envp);

	// t_argv *args1 = malloc(sizeof(t_argv));
	// t_argv *args2 = malloc(sizeof(t_argv));
	// args1->content = "NAME1=bkol";
	// args1->next = args2;
	// args2->content = "NAME2=kolani";
	// args2->next = NULL;
	// args_exp = args1;
	// export_env_var(env, args_exp);
	// print_env_vars(env);
	// t_argv *args_1 = malloc(sizeof(t_argv));
	// t_argv *args_2 = malloc(sizeof(t_argv));
	// args_1->content = "NAME1";
	// args_1->next = args_2;
	// args_2->content = "NAME7";
	// args_2->next = NULL;
	// args_unset = args_1;
	// unset_env_var(env, args_unset);
	// print_env_vars(env);
	// print_env_vars(env);
	// export_env_var(env, "NAME2=ani");
	// export_env_var(env, "NAME5=kol");
	// export_env_var(env, "NAME6=kol");
	// export_env_var(env, "NAME7=kol");
	// export_env_var(env, "NAME3=ani");
	// print_env_vars(env);
	// unset_env_var(env, "NAME5");
	// print_env_vars(env);
	// while (env)
	// {
	// 	free(env->full);
	// 	free(env->key);
	// 	free(env->value);
	// 	free(env);
	// 	env = env->next;
	// }
	// export_with_no_args(env);

	// handle signals:
	// handle_signals();
	// t_argv *cd_arg;
	// cd_arg = malloc(sizeof(t_argv));
	// cd_arg->content = "Desktop";
	// cd_arg->next = NULL;
	// cd(env, NULL);
	// pwd();
	// cd(env, cd_arg);
	// cd(env, "mfjdjhd");
	// pwd();
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
	t_argv *echo_args =  args1;
	echo(echo_args);
	loop_minishell();
}