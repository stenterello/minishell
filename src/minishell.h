/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:54:58 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/14 14:32:23 by ddelladi         ###   ########.fr       */
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
	int					suspended_cat;
	int					is_suspended;
	int					top;
	int					terminated;
	int					signaled;
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
void	sup_loop(t_command cmd);
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
int		define_input(char *line, t_command *cmd);
void	define_output(char *line, t_command *cmd);
void	define_append_output(char *line, t_command *cmd);
void	define_pipe(t_command *cmd);
void	define_pipe_to(t_command *cmd);
void	define_heredoc_pipe(t_command *cmd);
void	restore_fd(t_command *cmd);
int		is_redir(char *line);
int		is_heredoc(char *line);
void	malloc_c(char **dst, int len);
void	malloc_c_ptr(char ***dst, int len);
void	malloc_and_check_dict(t_dict **dst, int len);
int		treat_heredoc(char *typed);
char	*take_delimiter(char *line);
void	clean_heredoc(char *line, char *bench);
int		count_params(char *line);
int		count_args(char **tmp);
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
void	take_variable(char *var1, t_input *input, int init_len);
int		count_units(char *line, int i);
int		fill_cmd_fields(char **tmp, t_command *cmd, int start);
void	print_here(char *delimiter, int i);
void	split_var_decl(char *line, t_command *cmd);
int		count_fields(char *s, char c);
int		find_start(char *s, char c);
int		count_len(char *s, char c);
int		find_next_start(char *s, char c);
int		is_logical_token(char c);
int		syntax_error_no_arr(void);
void	transform_environ(t_dict *env);
void	free_here(char *tmp, char *delimiter, t_command *cmd, t_command *cmd2);
int		sup_check(char *typed, int i, t_input *input, int *open);
void	sup_check2(char *typed, t_input *input, int *open, int i);
char	*define_var_name(char *line);
int		until_end_var_name(char *line, char *var);
int		until_dollar(char *line);
int		check_wildcards(t_command *cmd);
int		has_wildcard(char *str);
int		is_wildcard(char c);
int		is_in(char *range, char c);
int		is_verified_brackets(char *brackets, char file_char);
int		count_portions(char *line);
int		count_range(char a, char b);
int		count_letters(char *brackets);
int		count_results(char **portions);
int		count_h_results(char **portions);
int		take_brackets_param(char *line, char **portion);
void	insert_brackets_param(char *line, char *portion, int len);
char	*chrs_range(char a, char b);
char	*get_letters(char *brackets);
int		try_parse_brackets(char *line);
void	take_string_portion(char *line, char **portion);
char	**get_results(char **portions, int len);
char	**get_hidden_results(char **portions, int len);
int		is_verified(char *file, char **portions);
int		helper_guess(t_command *cmd);
int		check_export(t_command *cmd);
void	fill_prev(t_command *cmd, int *c, char **tmp);
void	fill_next(t_command *cmd, int *c, char **tmp);
void	cpy_and_slide(char **tmp, int *c, int start, t_command *cmd);
int		count_cleaned_cmd(char **tmp);
int		sup_check_red(char **tmp, int i, t_command *cmd);
int		guess(t_command *cmd, int i);
void	c_run(char **argv);
void	fd_error(char *line);
void	read_and_execute(char *line, t_command *cmd);
void	script_run(char **argv);
void	suspended_cat(void);
void	treat_suspended_cat(t_command *tmp);
void	treat_heredoc_child(int *status, t_command *tmp);
int		cmd_exists(char *line);
int		skip_spaces(char *line, int i);
int		n_lvls(char **l);
void	create_exits(int **exits, char **u_lines);
void	increment_couple(int *f, int *s);
void	sup_count_units(char *line, int *i, int *ret);
void	unexpected(char *typed);
void	sup_take_input(char *typed, char *tmp, t_input *input);
void	take_bonus(char *line, int *i, char c, int *ret);
void	take_elem(t_dict *elem, int *ind);
int		var_to_append(char *line);
t_dict	*try_search(char *key, t_dict *where);
t_dict	*find_ptr(char *key);
void	append_var(char *key, char *value);
void	sup_export(t_command *cmd, t_dict *new, int i);
int		preliminary(t_command *tmp);
void	next_level(void);
int		expand_if_sup_exp(t_dict *new, t_command *cmd);
void	sup_export(t_command *cmd, t_dict *new, int i);
void	insert_empty_var(t_dict *new);
void	sup_sup_export(t_dict *new);
int		extend_sup_exp(t_dict *new, int i, t_command *cmd, int j);
int		permitted(t_command *tmp);
void	restore_all(t_command *cmd);
int		infinite_exit(t_command *tmp);
char	**sup_h_res(struct dirent *en, char **ret, char **port, DIR *stream);
int		free_guess(char **tmp, t_command *cmd);
int		sup_guess(t_command *cmd, int i, int j, int k);
int		free_hidden_guess(char **tmp, t_command *cmd);
void	free_single_command(t_command *cmd);
void	sup1_sup1(t_command *cmd, char *tmp);

#endif
