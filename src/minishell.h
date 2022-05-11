/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:54:58 by ddelladi          #+#    #+#             */
/*   Updated: 2022/05/11 13:16:26 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../include/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <term.h>

typedef struct s_input
{
	char	*line;
	int		s_quot;
	int		d_quot;
	int		to_expand;
	int		is_open;
	int		with_error;
	char	**args;
	char	*expanded;
}				t_input;

typedef struct s_dict
{
	char	*key;
	char	*value;
	void	*next;
}				t_dict;

typedef struct s_term
{
	t_dict				*env;
	t_dict				*var;
	struct termios		*termi;
	struct termios		*old_term;
	int					last_exit;
	struct sigaction	acts;
	t_input				input;
	pid_t				child;
	int					delimiter;
	char				**glob_environ;
	// int					fd;
}				t_term;

typedef struct s_command
{
	char	*cmd;
	char	**args;
	char	*input_line;
	void	*next;
	void	*prev;
	int		redir_in;
	int		redir_out;
	int		saved_in;
	int		saved_out;
	int		input_fd;
	int		output_fd;
	int		to_pipe;
	int		to_pipe_to;
	int		first;
	char	**portions;
}				t_command;

t_term	g_term;

void	main_loop(void);
void	die(char *msg);
void	try_expand(t_input *input);
void	execute_tree(t_command *cmd);
char	*get_path(char *line);
char	*ft_getenv(char *line);
char	*pwd(void);
void	echo(t_command *cmd);
void	cd(t_command *cmd);
void	exit_cmd(t_command *cmd);
void	env(t_command *cmd);
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
int		split_command(char *line, t_command *cmd);
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
int		check_redirection(char **tmp, t_command *cmd);
void	check_pipe(char *line, t_command *cmd);
void	define_input(char *line, t_command *cmd);
void	define_output(char *line, t_command *cmd);
void	define_append_output(char *line, t_command *cmd);
void	define_pipe(t_command *cmd);
void	define_pipe_to(t_command *cmd);
void	define_heredoc_pipe(t_command *cmd);
void	restore_fd(t_command *cmd);
int		is_redir(char *line);
int		is_heredoc(char *line);
void	add_stop(void);
void	malloc_c(char **dst, int len);
void	malloc_c_ptr(char ***dst, int len);
void	malloc_and_check_dict(t_dict **dst, int len);
void	malloc_and_check_list(t_list **dst, int len);
int		treat_heredoc(char *typed);
char	*take_delimiter(char *line);
void	clean_heredoc(char *line, char *bench);
int		to_continue(char *typed, char *delimiter);
void	write_to_stdin(char *line);
int		count_params(char *line);
int		count_args(char **tmp);
int		count_asterisks(char *line);
int		dif(char *s1, char *s2);
int		next_arg_len(char *line);
void	save_term(struct termios **terminal);
void	reset_term(void);
int		is_directory(t_command *cmd);
void	treat_var_decl(t_command *cmd);
void	rewrite_args(t_command *cmd);
char	*last_field(char *line);
char	**split_fields(char *s, char c);
int		change_exist_var_in_dict(char *key, char *value, t_dict *where);
void	insert_into_vars(char *key, char *value, t_dict *where);
char	*search_vars(char *var_name, t_dict *where);
void	free_array_of_array(char **arr);
void	born_child(t_command *tmp);
int		is_logical(char *line);
void	get_logical(char *line, t_command *cmd);
void	free_commands(t_command *cmd);
int		builtin(t_command *cmd);
void	sup_sup_cd1(t_command *old, char *act);
void	sup_sup_cd2(t_command *old, char *act);
void	sup_sup_cd3(char *act, t_command *old);
void	sup_born(t_command *tmp, int status);
int		sup_ex(t_command *tmp);
void	define_pipe(t_command *cmd);
void	define_pipe_to(t_command *cmd);
void	define_heredoc_pipe(t_command *cmd);
void	take_variable(char *var1, t_input *input, int init_len);
int		next_unit(char *line);
int		unit_len(char *line);
int		count_units(char *line);
int		is_logical(char *line);
int		*define_cmds_logic(char *line);
int		fill_cmd_fields(char **tmp, t_command *cmd, int start);
void	syntax_error(char **tmp);
void	print_here(char *delimiter, int i);
void	split_var_decl(char *line, t_command *cmd);
int		count_fields(char *s, char c);
int		find_start(char *s, char c);
int		count_len(char *s, char c);
int		find_next_start(char *s, char c);
int		is_logical_token(char c);
int		is_in_par(char *line, int limit);
int		syntax_error_no_arr(void);
void	transform_environ(t_dict *env);
void	store_new_args(char **args, char **files, char **ret);
int		key_len(char *line);
int		value_len(char *line);
void	sup1(char *delimiter, t_command *cmd, char *tmp, t_command *cmd2);
int		sup_check(char *typed, int i, t_input *input, int *open);
void	sup_check2(char *typed, t_input *input, int *open, int i);
int		sup_unit_len(char *line, int i);
char	*define_var_name(char *line);
int		until_end_var_name(char *line, char *var);
int		until_dollar(char *line);
int		no_output(char *line);
int		syntax_error_no_arr(void);
int		empty_input(char *line);
int		empty_output(char *line);
char	*get_filename(char *line);
int		check_wildcards(t_command *cmd);
int		has_wildcard(char *str);
int		is_wildcard(char c);
int		ft_islower(char c);
int		is_in(char *range, char c);
int		is_verified_brackets(char *brackets, char file_char);
int		count_portions(char *line);
int		count_range(char a, char b);
int		count_letters(char *brackets);
int		count_results(char **portions);
int		take_brackets_param(char *line, char **portion);
void	insert_brackets_param(char *line, char *portion, int len);
char	*chrs_range(char a, char b);
char	*get_letters(char *brackets);
int		try_parse_brackets(char *line);
void	take_string_portion(char *line, char **portion);
char	**get_results(char **portions, int len);
int		is_verified(char *file, char **portions);
int		helper_guess(t_command *cmd);

#endif
