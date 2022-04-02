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
void	cd(char *line);
int		is_abs_path(char *line);
char	*get_path(char *line);
int		find_script(char *line);
int		search_in_dir(DIR *stream, char *line, char *dir_name);

#endif
