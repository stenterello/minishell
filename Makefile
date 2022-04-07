NAME=minishell
CC=gcc
FLAGS=-Wall -Werror -Wextra -g 
SRC=main.c \
	utils.c \
	execute.c \
	execute_utils.c
SRCS=$(addprefix src/, $(SRC))
OBJS=$(SRCS:.c=.o)
BUILTIN=pwd.c \
		cd.c \
		echo.c \
		exit.c
BUILTINS=$(addprefix src/builtin/, $(BUILTIN))
BUILTIN_OBJS=$(BUILTINS:.c=.o)
LIBFT=./include/libft/libft.a

$(NAME): $(LIBFT) $(OBJS) $(BUILTIN_OBJS)
	$(CC) $(FLAGS) $(SRCS) $(BUILTINS) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
	$(MAKE) -C ./include/libft

all: $(NAME)

clean:
	rm -f $(BUILTINS_OBJS) $(OBJS)
	$(MAKE) clean -C ./include/libft

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C ./include/libft

re: fclean all

.PHONY: all clean fclean re
