NAME=minishell
CC=gcc
FLAGS_LIN=-Wall -Werror -Wextra -g 
FLAGS=-Wall -Werror -Wextra -g -L$(HOME)/.brew/opt/readline/lib -I$(HOME)/.brew/opt/readline/include
SRC=main.c \
	utils.c \
	execute.c \
	find_script.c \
	init_utils.c \
	set_var.c \
	term.c \
	parse.c \
	bools.c \
	bools2.c \
	count.c \
	memory.c \
	redirections.c \
	redirections_utils.c \
	heredoc.c \
	split_fields.c \
	signal.c \
	error_handling.c \
	logical.c
SRCS=$(addprefix src/, $(SRC))
OBJS=$(SRCS:.c=.o)
BUILTIN=pwd.c \
		cd.c \
		echo.c \
		exit.c \
		env.c \
		export.c \
		unset.c
BUILTINS=$(addprefix src/builtin/, $(BUILTIN))
BUILTIN_OBJS=$(BUILTINS:.c=.o)
LIBFT=./include/libft/libft.a

$(NAME): $(LIBFT) $(OBJS) $(BUILTIN_OBJS)
	$(CC) $(FLAGS) $(SRCS) $(BUILTINS) $(LIBFT) -lreadline -lncurses -o $(NAME) 

lin: $(LIBFT) $(OBJS) $(BUILTIN_OBJS)
	$(CC) $(FLAGS_LIN) $(SRCS) $(BUILTINS) $(LIBFT) -o $(NAME) -lreadline -lncurses

$(LIBFT):
	$(MAKE) bonus -C ./include/libft

all: $(NAME)

clean:
	rm -f $(BUILTIN_OBJS) $(OBJS)
	$(MAKE) clean -C ./include/libft

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C ./include/libft

re: fclean all

.PHONY: all clean fclean re
