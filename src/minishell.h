#ifndef MINISHELL_H
# define MINISHELL_H

# include "../includes/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/types.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/wait.h>

void	die(char *msg);
char	*pwd(void);
void	cd(char **args);
void	echo(char **args);
int		is_abs_path(char *line);
char	*get_path(char *line);
char	*get_full_path(char *dir_name, char *name);
int		find_script(char **args);
int		search_in_dir(DIR *stream, char **args, char *dir_name);
int		go_nl(char *line);
void	cmd_not_found(char *line);
int		ft_isupper(char c);
char	*get_variable(char *line);
void	elaborate_cmd(char *line);
int		dollar_pos(char *line);
int		single_quoted(char *line, int ind);

#endif
