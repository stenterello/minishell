NAME=minishell
CC=gcc
FLAGS=-Wall -Werror -Wextra -L$(HOME)/.brew/opt/readline/lib -I$(HOME)/.brew/opt/readline/include -g
SRC=main.c \
	main2.c \
	special_run.c \
	special_childs.c \
	utils.c \
	utils2.c \
	utils3.c \
	execute.c \
	execute2.c \
	execute3.c \
	execute4.c \
	find_script.c \
	init_utils.c \
	init_utils2.c \
	init_utils3.c \
	set_var.c \
	set_var2.c \
	term.c \
	parse.c \
	parse2.c \
	parse3.c \
	bools.c \
	bools2.c \
	bools3.c \
	bools4.c \
	count.c \
	memory.c \
	redirections.c \
	redirections_utils.c \
	redirections_utils2.c \
	heredoc.c \
	heredoc2.c \
	heredoc3.c \
	split_fields.c \
	split_fields2.c \
	signal.c \
	error_handling.c \
	logical.c \
	logical2.c \
	logical3.c \
	logical4.c \
	wildcards.c \
	wildcards2.c \
	wildcards_bools.c \
	wildcards_bools2.c \
	wildcards_count.c \
	wildcards_count2.c \
	wildcards_utils.c \
	wildcards_utils2.c \
	wildcards_utils3.c 
SRCS=$(addprefix src/, $(SRC))
OBJS=$(SRCS:.c=.o)
BUILTIN=pwd.c \
		cd.c \
		cd2.c \
		echo.c \
		exit.c \
		env.c \
		export.c \
		export2.c \
		export3.c \
		unset.c
BUILTINS=$(addprefix src/builtin/, $(BUILTIN))
BUILTIN_OBJS=$(BUILTINS:.c=.o)
LIBFT=./include/libft/libft.a

$(NAME): $(LIBFT) $(OBJS) $(BUILTIN_OBJS)
	$(CC) $(FLAGS) $(SRCS) $(BUILTINS) $(LIBFT) -lreadline -lncurses -o $(NAME) 

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
