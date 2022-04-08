#ifndef MINISHELL_H
# define MINISHELL_H

# include "../include/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/types.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <termios.h>
# include <term.h>

typedef struct	s_input
{
	char	*line;
	int		s_quot;
	int		d_quot;
	int		to_expand;
	int		is_open;
	char	**args;
	char	*expanded;
}				t_input;

typedef struct	s_env_elem
{
	char	*key;
	char	*value;
	void	*next;
	void	*prev;
	int		start;
}				t_env_elem;


typedef struct	s_term
{
	t_env_elem		*env;
	struct termios	*termi;
	int				last_exit;
}				t_term;


typedef struct	s_command
{
	char	*cmd;
	char	*opt;
	char	**args;
	int		stdin;
	int		stdout;
	int		stderr;
}				t_command;

extern char **environ;

void	die(char *msg);
void	try_expand(t_input *input, t_term *term);
void	execute(t_input *input);
char	*get_path(char *line);
char	*pwd(void);
void	echo(t_input *input);
void	cd(char **args);
void	exit_cmd(char **args);
void	env(t_term *g_term);
void	export(t_input *input);
void	cmd_not_found(char *line);
int		find_script(char **args);
int		builtin(t_input *input);
void	init_terminal(char *line, t_term *term);
void	init_input(t_input *input);
void	take_environ(t_term *term);
void	take_input(t_input *input);
void	free_env(t_env_elem *env);

#endif
