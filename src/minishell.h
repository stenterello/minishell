#ifndef MINISHELL_H
#define MINISHELL_H

#define _XOPEN_SOURCE 700

#include "../include/libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <termios.h>
#include <term.h>

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
	int 				last_exit;
	struct sigaction	acts;
	t_input				input;
}				t_term;

typedef struct	s_command
{
	char	*cmd;
	char	*opt;
	char	**args;
	void	*next;
	int		redir_stdin;
	int		redir_stdout;
	int		redir_stderr;
	int		saved_in;
	int		saved_out;
	int		saved_err;
	int		fd;
}				t_command;

t_term	g_term;

void	die(char *msg);
void	try_expand(t_input *input);
void	execute(t_command *cmd);
char	*get_path(char *line);
char	*ft_getenv(char *line);
char	*pwd(void);
void	echo(t_command *cmd);
void	cd(t_command *cmd);
void	exit_cmd(t_command *cmd);
void	env(void);
void	export(t_command *cmd);
void	unset(t_command *cmd);
void	cmd_not_found(t_command *cmd);
int		find_script(t_command *cmd);
int		builtin(t_command *cmd);
void	init_terminal(char *line);
void	init_input_and_cmd(t_input *input, t_command *cmd);
void	take_environ(void);
void	take_input(t_input *input);
void	split_command(char *line, t_command *cmd);
void	free_env(t_env_var *env);
void	free_sh(t_sh_var *var);
int		key_len(char *line);
int		value_len(char *line);
int		is_open(char *typed, int limit);
int		is_var_def(char *line);
int		set_env_var(char **args);
void	set_sh_var(char **args);
void	check(char *typed, t_input *input);
void	add_signals(void);
void	define_input(char *line, t_command *cmd);
void	define_output(char *line, t_command *cmd);
void	define_append_output(char *line, t_command *cmd);
void	restore_fd(t_command *cmd);
int		is_redir(char *line);
int		is_heredoc(char *line);

#endif
