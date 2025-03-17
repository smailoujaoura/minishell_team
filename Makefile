NAME = minishell
LIBFT := ./utils/libft/ft_atoi.c \
		./utils/libft/ft_bzero.c \
		./utils/libft/ft_calloc.c \
		./utils/libft/ft_isalnum.c \
		./utils/libft/ft_isalpha.c \
		./utils/libft/ft_isascii.c \
		./utils/libft/ft_isdigit.c \
		./utils/libft/ft_isprint.c \
		./utils/libft/ft_itoa.c \
		./utils/libft/ft_memchr.c \
		./utils/libft/ft_memcmp.c \
		./utils/libft/ft_memcpy.c \
		./utils/libft/ft_memmove.c \
		./utils/libft/ft_memset.c \
		./utils/libft/ft_putchar_fd.c \
		./utils/libft/ft_putendl_fd.c \
		./utils/libft/ft_putnbr_fd.c \
		./utils/libft/ft_putstr_fd.c \
		./utils/libft/ft_split.c \
		./utils/libft/ft_strchr.c \
		./utils/libft/ft_strdup.c \
		./utils/libft/ft_striteri.c \
		./utils/libft/ft_strjoin.c \
		./utils/libft/ft_strlcat.c \
		./utils/libft/ft_strlcpy.c \
		./utils/libft/ft_strlen.c \
		./utils/libft/ft_strmapi.c \
		./utils/libft/ft_strncmp.c \
		./utils/libft/ft_strnstr.c \
		./utils/libft/ft_strrchr.c \
		./utils/libft/ft_strtrim.c \
		./utils/libft/ft_substr.c \
		./utils/libft/ft_tolower.c \
		./utils/libft/ft_toupper.c \
		./utils/libft/get_next_line.c \
		./utils/libft/get_next_line_utils.c \
		./utils/libft/allocator.c \
		./utils/libft/ft_lstadd_back_bonus.c \
		./utils/libft/ft_lstadd_front_bonus.c \
		./utils/libft/ft_lstclear_bonus.c \
		./utils/libft/ft_lstdelone_bonus.c \
		./utils/libft/ft_lstiter_bonus.c \
		./utils/libft/ft_lstlast_bonus.c \
		./utils/libft/ft_lstmap_bonus.c \
		./utils/libft/ft_lstnew_bonus.c \
		./utils/libft/ft_lstsize_bonus.c

SRCS =	main.c \
		minishell.c \
		./utils/list_utils.c \
		./utils/list_utils_arg.c \
		./utils/garbage_collector.c \
		./utils/bkol_garb_coll.c \
		./utils/libft/allocator.c \
		./utils/signals.c \
		./parser/lexing/lexer.c \
		./parser/lexing/lexer_utils1.c \
		./parser/parsing/parser_utils_1.c \
		./parser/parsing/parser_utils_2.c \
		./parser/parsing/parser_utils_3.c \
		./parser/parsing/parser_utils_4.c \
		./parser/parsing/parser_utils_5.c \
		./parser/parsing/syntax_validator.c \
		./parser/parsing/syntax_validator_helper.c \
		./parser/parsing/ast_tree.c \
		./parser/heredoc.c \
		./builtins/cd.c \
		./builtins/echo.c \
		./builtins/env.c \
		./builtins/exit.c \
		./builtins/export.c \
		./builtins/pwd.c \
		./builtins/unset.c \
		./builtins/utils_1.c \
		./builtins/utils_2.c \
		./builtins/export_utils.c \
		./expander/expander.c \
		./expander/expanding_heredoc.c \
		./expander/expanding_utils_1.c \
		./expander/expanding_utils_2.c \
		./expander/expanding_utils_3.c \
		./expander/expanding_utils_4.c \
		./expander/expanding_utils_5.c \
		./expander/expanding_utils_6.c \
		./expander/split.c \
		./executor/here_doc.c \
		./executor/executor.c \
		./executor/executor_other.c \
		./executor/executor_redirs.c \
		./executor/executor_utils.c \
		./executor/sys_calls.c \
		./executor/and_or.c

OBJS =  $(SRCS:.c=.o)
DEPENS = $(SRCS:.c=.d)

CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBS = ./utils/libft/libft.a

all: $(NAME)

%.o: %.c ./includes/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -lreadline -lncurses -o $(NAME)

$(LIBS): ./utils/libft/libft.h $(LIBFT)
	make -C utils/libft
	make -C utils/libft bonus

bonus: all

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

.PHONY: clean