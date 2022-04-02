NAME=minishell
CC=gcc
FLAGS=-Wall -Werror -Wextra -g
LIBFT=./includes/libft/libft.a
SRC=main.c \
	utils.c \
	utils_2.c \
	get_input_utils.c \
	expand.c \
	elaborate_command.c
SRCS=$(addprefix src/, $(SRC))
BUILTIN=pwd.c \
		cd.c \
		echo.c
BUILTINS=$(addprefix src/builtin/, $(BUILTIN))
OBJ=$(SRC:.c=.o)
OBJS=$(SRCS:.c=.o)
BUILTIN_OBJ=$(BUILTIN:.c=.o)
BUILTIN_OBJS=$(BUILTINS:.c=.o)

$(NAME): $(LIBFT) $(OBJS) $(BUILTIN_OBJS)
	$(CC) $(FLAGS) $(SRCS) $(BUILTINS) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
	$(MAKE) bonus -C ./includes/libft

$(OBJS):
	$(CC) $(FLAGS) -c $(SRCS)
	mv $(OBJ) src/

$(BUILTIN_OBJS):
	$(CC) $(FLAGS) -c $(SRCS) $(BUILTINS)
	mv $(BUILTIN_OBJ) src/builtin

all: $(NAME)

clean:
	rm -f $(OBJS)
	rm -f $(BUILTIN_OBJS)
	$(MAKE) clean -C ./includes/libft

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C ./includes/libft

re: fclean all

.PHONY: all clean fclean re
