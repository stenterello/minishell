#ifndef MINISHELL_H
# define MINISHELL_H

# include "../includes/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

char	*ft_strjoin_plus(char *s1, char *s2);
char	*pwd(void);
void	cd(char *line);

#endif
