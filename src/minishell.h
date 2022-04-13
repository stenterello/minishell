#ifndef MINISHELL_H
# define MINISHELL_H

# define _XOPEN_SOURCE 700

# include "../include/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/types.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
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

typedef struct	s_env_var
{
	char	*key;
	char	*value;
	void	*next;
	void	*prev;
	int		start;
}				t_env_var;

typedef struct	s_sh_var
{
	char	*key;
	char	*value;
	void	*next;
}				t_sh_var;

typedef struct	s_term
{
	t_env_var			*env;
	t_sh_var			*var;
	struct termios		*termi;
	int					last_exit;
	struct sigaction	acts;
	t_input				input;
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

//extern char	**environ;

void	die(char *msg);
void	try_expand(t_input *input, t_term *term);
void	execute(t_command *cmd, t_term *term);
char	*get_path(char *line);
char	*ft_getenv(char *line, t_term *term);
char	*pwd(t_term *term);
void	echo(t_command *cmd, t_term *term);
void	cd(t_command *cmd, t_term *term);
void	exit_cmd(t_command *cmd);
void	env(t_term *term);
void	export(t_command *cmd, t_term *term);
void	unset(t_command *cmd, t_term *term);
void	cmd_not_found(t_command *cmd, t_term *term);
int		find_script(t_command *cmd, t_term *term);
int		builtin(t_command *cmd, t_term *term);
void	init_terminal(char *line, t_term *term);
void	init_input_and_cmd(t_input *input, t_command *cmd);
void	take_environ(t_term *term);
void	take_input(t_input *input);
void	split_command(char *line, t_command *cmd);
void	free_env(t_env_var *env);
void	free_sh(t_sh_var *var);
int		key_len(char *line);
int		value_len(char *line);
int		is_open(char *typed, int limit);
int		is_var_def(char *line);
int		set_env_var(char **args, t_term *term);
void	set_sh_var(char **args, t_term *term);
void	check(char *typed, t_input *input);
void	add_signals(t_term *term);

#endif
