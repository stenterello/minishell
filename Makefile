NAME=minishell
CC=gcc
FLAGS=-Wall -Werror -Wextra -L$(HOME)/.brew/opt/readline/lib -I$(HOME)/.brew/opt/readline/include
SRC=main.c \
	special_run.c \
	special_childs.c \
	utils.c \
	utils2.c \
	utils3.c \
	dict_utils.c \
	execute.c \
	execute2.c \
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
	parse4.c \
	bools.c \
	bools2.c \
	bools3.c \
	bools4.c \
	count.c \
	memory.c \
	memory2.c \
	redirections.c \
	redirections_utils.c \
	heredoc.c \
	heredoc2.c \
	split_fields.c \
	split_fields2.c \
	signal.c \
	signal2.c \
	error_handling.c \
	chain.c \
	chain2.c \
	chain3.c \
	wildcards.c \
	wildcards2.c \
	wildcards_bools.c \
	wildcards_bools2.c \
	wildcards_count.c \
	wildcards_count2.c \
	wildcards_utils.c \
	wildcards_utils2.c \
	wildcards_utils3.c \
	child_process.c \
	child_process2.c
SRCS=$(addprefix src/, $(SRC))
OBJS=$(SRCS:.c=.o)
BUILTIN=pwd.c \
		cd.c \
		cd2.c \
		echo.c \
		exit.c \
		env.c \
		export.c \
		export_utils.c \
		unset.c
BUILTINS=$(addprefix src/builtin/, $(BUILTIN))
BUILTIN_OBJS=$(BUILTINS:.c=.o)
LIBFT=./include/libft/libft.a

$(NAME): $(LIBFT) $(OBJS) $(BUILTIN_OBJS)
	$(CC) $(FLAGS) $(OBJS) $(BUILTIN_OBJS) $(LIBFT) -lreadline -lncurses -o $(NAME) 

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
