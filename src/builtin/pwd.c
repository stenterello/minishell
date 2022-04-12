#include "../minishell.h"

char	*pwd(t_term *term)
{
	char	*path;
	int		i;

	i = 1;
	path = NULL;
	path = getcwd(path, i);
	while (path == NULL)
	{
		free(path);
		i++;
		path = getcwd(path, i);
	}
	term->last_exit = 0;
	return (path);
}
