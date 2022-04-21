#ifndef MINISHELL_H
#define MINISHELL_H

#include "../include/libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/stat.h>
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

typedef struct	s_dict
{
	char	*key;
	char	*value;
	void	*next;
}				t_dict;

typedef struct	s_term
{
	t_dict				*env;
	t_dict				*var;
	struct termios		*termi;
	struct termios		*old_term;
	int 				last_exit;
	struct sigaction	acts;
	t_input				input;
	pid_t				child;
	int					delimiter;
}				t_term;

typedef struct	s_command
{
	char	*cmd;
	char	*opt;
	char	**args;
	void	*next;
	void	*prev;
	int		redir_stdin;
	int		redir_stdout;
	int		redir_stderr;
	int		saved_in;
	int		saved_out;
	int		saved_err;
	int		fd;
	int		to_pipe;
	int		to_pipe_to;
	int		piped_fd;
	int		first;
}				t_command;

t_term	g_term;

void	main_loop(void);
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
void	init_input(t_input *input);
void	init_cmd(t_command *cmd);
void	take_environ(void);
void	take_input(t_input *input);
void	split_command(char *line, t_command *cmd);
void	free_dict(t_dict *env);
int		key_len(char *line);
int		value_len(char *line);
int		is_open(char *typed, int limit);
int		is_var_def(char *line);
int		is_token(char *line);
int		quoted(char *line);
int		set_env_var(char **args);
void	set_sh_var(char **args);
void	check(char *typed, t_input *input);
void	add_signals(void);
void	check_redirection(char *line, t_command *cmd);
void	check_pipe(char *line, t_command *cmd);
void	define_input(char *line, t_command *cmd);
void	define_output(char *line, t_command *cmd);
void	define_append_output(char *line, t_command *cmd);
void	define_pipe(t_command *cmd);
void	define_pipe_to(t_command *cmd);
void	restore_fd(t_command *cmd);
int		is_redir(char *line);
int		is_heredoc(char *line);
void	kill_proc(int sig, siginfo_t *info, void *context);
void	malloc_and_check_char(char **dst, int len);
void	malloc_and_check_char_ptr(char ***dst, int len);
void	malloc_and_check_dict(t_dict **dst, int len);
void	malloc_and_check_list(t_list **dst, int len);
int		treat_heredoc(char *typed, t_input *input);
char	*take_delimiter(char *line);
void	clean_heredoc(char *line, char *bench);
int		to_continue(char *typed, char *delimiter);
void	write_to_stdin(char *line);
int		count_params(char *line);
int		count_args(char **tmp);
int		next_arg_len(char *line);
void	save_term(struct termios **terminal);
void	reset_term(void);

#endif
