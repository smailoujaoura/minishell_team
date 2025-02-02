NAME = minishell
SRCS = main.c list_utils.c env_handler.c tree.c
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

re: fclean all