NAME = minishell
SRCS =	main.c \
		minishell.c \
		./utils/list_utils.c \
		./parser/lexing/lexer.c \
		./parser/lexing/lexer_utils1.c \
		./parser/parsing/parser_utils_1.c \
		./parser/parsing/parser_utils_2.c \
		./parser/parsing/parser_utils_3.c \
		./parser/parsing/parser_utils_4.c \
		./parser/parsing/syntax_validator.c \
		./parser/parsing/syntax_validator_helper.c \
		./parser/parsing/ast_tree.c \
		./utils/garbage_collector.c \
		./utils/bkol_garb_coll.c \
		./utils/libft/allocator.c \
		./builtins/cd.c \
		./builtins/echo.c \
		./builtins/env.c \
		./builtins/exit.c \
		./builtins/export.c \
		./builtins/pwd.c \
		./builtins/unset.c \
		./builtins/utils_1.c \
		./builtins/utils_2.c \
		./builtins/utils_3.c \
		./expander/expander.c \
		./expander/expanding_heredoc.c \
		./expander/expanding_utils_1.c \
		./expander/expanding_utils_2.c \
		./expander/expanding_utils_3.c \
		./expander/expanding_utils_4.c \
		./expander/expanding_utils_5.c \
		./expander/expanding_utils_6.c \
		./executor/here_doc.c \
		./executor/executor.c \
		./executor/executor_other.c \
		./executor/executor_redirs.c \
		./executor/executor_utils.c

OBJS =  $(SRCS:.c=.o)
DEPENS = $(SRCS:.c=.d)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
LIBS = ./utils/libft/libft.a

all: $(NAME)

%.o: %.c minishell.h
	$(COMP) -c $< -o $@

$(NAME): $(LIBS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -lreadline -lncurses -o $(NAME)

$(LIBS):
	make -C utils/libft
	make -C utils/libft bonus

clean: 
	rm -rf $(OBJS)
	rm -rf $(DEPENS)
	make -C utils/libft clean

fclean: clean
	rm -rf $(NAME)
	make -C utils/libft fclean

run:
	./minishell

rer: re run

re: fclean all