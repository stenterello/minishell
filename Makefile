NAME=minishell
CC=gcc
FLAGS=-Wall -Werror -Wextra -g
LIBFT=./includes/libft/libft.a
SRC=main.c \
	utils.c
SRCS=$(addprefix src/, $(SRC))
BUILTIN=pwd.c \
		cd.c
BUILTINS=$(addprefix src/builtin/, $(BUILTIN))
OBJS=$(SRCS:.c=.o)
BUILTIN_OBJS=$(BUILTINS:.c=.o)

$(NAME): $(LIBFT) $(OBJS) $(BUILTIN_OBJS)
	$(CC) $(FLAGS) $(OBJS) $(BUILTIN_OBJS) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
	$(MAKE) -C ./includes/libft

all: $(NAME)

clean:
	rm -f $(OBJS)
	$(MAKE) clean -C ./includes/libft

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C ./includes/libft

re: fclean all

.PHONY: all clean fclean re
