NAME = minishell
SRCS =	main.c \
		minishell.c \
		utils/list_utils.c \
		parser/lexing/lexer.c \
		parser/lexing/lexer_utils1.c \
		parser/parsing/parser.c \
		parser/parsing/parser2.c \
		parser/parsing/parser_utils1.c \
		parser/parsing/syntax_validator.c \
		parser/parsing/ast_tree.c \
		garbage_collector.c \
		bkol_garb_coll.c \
		utils/libft/allocator.c \
		executor/here_doc.c \
		executor/executor.c \
		executor/expander.c \
		builtins/cd.c \
		builtins/echo.c \
		builtins/env.c \
		builtins/exit.c \
		builtins/export.c \
		builtins/pwd.c \
		builtins/unset.c \
		builtins/utils_1.c \
		builtins/utils_2.c \
		builtins/utils_3.c

OBJS =  $(SRCS:.c=.o)
DEPENS = $(SRCS:.c=.d)
COMP = cc -Wall -Wextra -Werror -g -fsanitize=address
FLAGS = -lreadline -lncurses
LIBS = ./utils/libft/libft.a

all: $(NAME)

%.o: %.c minishell.h
	$(COMP) -c $< -o $@

$(NAME): $(LIBS) $(OBJS)
	$(COMP) $(OBJS) $(LIBS) $(FLAGS) -o $(NAME)

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