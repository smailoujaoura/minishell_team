NAME = minishell
SRCS =	main.c \
		minishell.c \
		env_handler.c \
		utils/list_utils.c \
		parser/lexing/lexer.c \
		parser/lexing/lexer_utils1.c \
		parser/parsing/parser.c \
		parser/parsing/parser2.c \
		parser/parsing/parser_utils1.c \
		parser/parsing/syntax_validator.c \
		parser/parsing/ast_tree.c \
		executor/builtins.c \
		bkolani.c \
		garbage_collector.c \
		bkol_garb_coll.c \
		utils/libft/allocator.c \
		executor/here_doc.c

OBJS =  $(SRCS:.c=.o)
COMP = cc -Wall -Wextra -Werror -g -fsanitize=address
FLAGS = -lreadline
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
	make -C utils/libft clean

fclean: clean
	rm -rf $(NAME)
	make -C utils/libft fclean

run:
	./minishell

rer: re run

re: fclean all