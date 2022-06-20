/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:54:58 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/20 13:31:23 by ddelladi         ###   ########.fr       */
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

typedef struct s_terminfo
{
	struct termios		*new_term;
	struct termios		*old_term;
	t_dict				*env;
	t_dict				*var;
	struct sigaction	acts[2];
	t_input				*input;
	char				**glob_environ;
	int					delimiter;
	int					suspended_cat;
	int					is_suspended;
	int					top;
	int					signaled;
	int					last_exit;
}				t_terminfo;

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

pid_t	g_child;

int		c_run(char **argv, t_terminfo *terminfo);
int		script_run(char **argv, t_terminfo *terminfo);
void	init_and_take_input(t_terminfo *terminfo);
void	execution_loop(t_terminfo *terminfo);
void	die(char *msg);
void	try_expand(t_terminfo *terminfo);
void	execute_tree(t_command *cmd, t_terminfo *terminfo);
char	*get_path(char *line);
char	*ft_getenv(char *line, t_terminfo *terminfo);
char	*pwd(t_terminfo *terminfo);
void	echo(t_command *cmd, t_terminfo *terminfo);
void	cd(t_command *cmd, t_terminfo *terminfo);
void	exit_cmd(t_command *cmd, t_terminfo *terminfo);
void	env(t_command *cmd, t_terminfo *terminfo);
void	export(t_command *cmd, t_terminfo *terminfo);
void	unset(t_command *cmd, t_terminfo *terminfo);
void	cmd_not_found(t_command *cmd, t_terminfo *terminfo);
int		find_script(t_command *cmd, t_terminfo *terminfo);
int		builtin(t_command *cmd, t_terminfo *terminfo);
void	init_terminal(char *line, t_terminfo *terminfo);
void	init_input(t_input *input);
void	init_cmd(t_command *cmd);
void	take_environ(t_terminfo *terminfo);
void	take_input(t_terminfo *terminfo);
int		split_command(char *line, t_command *cmd, t_terminfo *terminfo);
void	free_dict(t_dict *env);
int		key_len(char *line);
int		value_len(char *line);
int		is_open(char *typed, int limit);
int		is_var_def(char *line);
int		is_token(char *line);
int		quoted(char *line);
int		set_env_var(char **args);
void	set_sh_var(char **args, t_terminfo *terminfo);
void	check(char *typed, t_terminfo *terminfo);
void	add_signals(t_terminfo *terminfo, t_command *cmd);
int		check_redirection(char **tmp, t_command *cmd, t_terminfo *terminfo);
void	check_pipe(char *line, t_command *cmd);
int		define_input(char *line, t_command *cmd, t_terminfo *terminfo);
void	define_output(char *line, t_command *cmd);
void	define_append_output(char *line, t_command *cmd);
void	define_pipe(t_command *cmd);
void	define_pipe_to(t_command *cmd);
void	define_heredoc_pipe(t_command *cmd);
void	restore_fd(t_command *cmd, t_terminfo *terminfo);
void	restore_all(t_command *cmd, t_terminfo *terminfo);
int		is_redir(char *line);
int		is_heredoc(char *line);
void	malloc_c(char **dst, int len);
void	malloc_c_ptr(char ***dst, int len);
void	malloc_and_check_dict(t_dict **dst, int len);
void	malloc_and_copy(char **dst, char *src);
int		init_heredoc(char *typed, t_terminfo *terminfo);
void	treat_heredoc_child(int *status, t_command *tmp, t_terminfo *terminfo);
char	*take_delimiter(char *line);
void	clean_heredoc(char *line, char *bench);
int		count_params(char *line);
int		count_args(char **tmp);
int		next_arg_len(char *line);
void	save_term(struct termios **terminal);
void	reset_term(t_terminfo *terminfo);
int		is_directory(t_command *cmd, t_terminfo *terminfo);
void	treat_var_decl(t_command *cmd, t_terminfo *terminfo);
void	rewrite_args(t_command *cmd);
char	*last_field(char *line);
char	**split_fields(char *s, char c);
int		change_exist_var_in_dict(char *key, char *value, t_dict *where);
void	insert_into_vars(char *key, char *value, t_dict *where);
char	*search_vars_value(char *var_name, t_dict *where);
void	free_array_of_array(char **arr);
void	born_child(t_command *tmp, t_terminfo *terminfo);
int		is_chained(char *line);
void	get_keys(char *line, t_command *cmd, t_terminfo *terminfo);
void	free_commands(t_command *cmd);
void	save_old_pwd(t_command *old, char *act, t_terminfo *terminfo);
void	sup_born(t_command *tmp, int status, t_terminfo *terminfo);
int		standard_execution(t_command *tmp, t_terminfo *terminfo);
void	take_variable(char *var1, t_terminfo *terminfo, int init_len);
int		count_units(char *line, int i);
int		fill_cmd_fields(char **tmp, t_command *cmd,
			int start, t_terminfo *terminfo);
void	print_here(char *delimiter, int i, t_terminfo *terminfo);
void	split_var_decl(char *line, t_command *cmd);
int		count_fields(char *s, char c);
int		find_start(char *s, char c);
int		count_len(char *s, char c);
int		find_next_start(char *s, char c);
int		is_chaining_token(char c);
int		syntax_error_no_arr(t_terminfo *terminfo);
void	transform_environ(t_terminfo *terminfo);
void	free_here(char *tmp, char *delimiter, t_command *cmd, t_command *cmd2);
int		sup_check(char *typed, int i, t_terminfo *terminfo, int *open);
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
int		check_export_syntax(t_command *cmd, int ind, t_terminfo *terminfo);
void	fill_prev(t_command *cmd, int *c, char **tmp);
void	fill_next(t_command *cmd, int *c, char **tmp, t_terminfo *terminfo);
void	cpy_and_slide(char **tmp, int *c, int start, t_command *cmd);
int		count_cleaned_cmd(char **tmp);
int		sup_check_red(char **tmp, int i, t_command *cmd, t_terminfo *terminfo);
int		guess(t_command *cmd, int i);
void	fd_error(char *line, t_terminfo *terminfo);
void	read_and_execute(char *line, t_terminfo *terminfo);
void	suspended_cat(t_terminfo *terminfo);
void	treat_suspended_cat(t_command *tmp, t_terminfo *terminfo);
int		cmd_exists(char *line, t_terminfo *terminfo);
int		skip_spaces(char *line, int i);
int		n_lvls(char **l);
void	create_exits(int **exits, char **u_lines);
void	increment_couple(int *f, int *s);
void	sup_count_units(char *line, int *i, int *ret);
void	unexpected(char *typed, t_terminfo *terminfo);
void	sup_take_input(char *typed, char *tmp, t_terminfo *terminfo);
void	take_bonus(char *line, int *i, char c, int *ret);
void	take_elem(t_dict *elem, int *ind);
int		var_to_append(char *line);
t_dict	*try_search(char *key, t_dict *where);
t_dict	*find_ptr(char *key, t_terminfo *terminfo);
void	append_var(char *key, char *value, t_terminfo *terminfo);
void	understand_then_export(t_command *cmd, t_dict *new,
			int i[2], t_terminfo *terminfo);
int		preliminary(t_command *tmp, t_terminfo *terminfo);
void	next_level(t_terminfo *terminfo);
int		expand_if_sup_exp(t_dict *new, t_command *cmd, t_terminfo *terminfo);
void	insert_empty_var(t_dict *new, t_terminfo *terminfo);
void	export_to_env_vars(t_dict *new, t_terminfo *terminfo);
int		concatenate_value(t_dict *new, int i[2],
			t_command *cmd, t_terminfo *terminfo);
void	print_exported_env(t_terminfo *terminfo);
int		permitted(t_command *tmp, t_terminfo *terminfo);
int		infinite_exit(t_command *tmp);
char	**sup_h_res(struct dirent *en, char **ret, char **port, DIR *stream);
int		free_guess(char **tmp, t_command *cmd);
int		sup_guess(t_command *cmd, int i, int j, int k);
int		free_hidden_guess(char **tmp, t_command *cmd);
void	free_single_command(t_command *cmd);
void	sup1_sup1(t_command *cmd, char *tmp);
void	execute_free_here(char *tmp, t_command *cmd,
			t_command *cmd2, t_terminfo *terminfo);
void	search_value_for_key(t_command *cmd, int *i,
			int ind, t_terminfo *terminfo);
int		var_exists(t_dict *new, t_command *cmd, t_terminfo *terminfo);
int		key_len(char *line);
int		value_len(char *line);
int		equal_char_index(t_command *cmd, int ind);
void	save_concatenation(char *dst, char *src1,
			char *src2, t_dict *save_dict);
char	*take_key(char *str);
int		ft_strlen_rl(char *str);
int		key_here_len(char *str);
int		to_expand_str(char *str);
int		to_exp(char *str);
char	*take_heredoc_input(char *tmp, char *d,
			t_command *cmd, t_terminfo *terminfo);
void	update_pwd(t_terminfo *terminfo);
void	cd_error(char *dest, t_terminfo *terminfo);

#endif
