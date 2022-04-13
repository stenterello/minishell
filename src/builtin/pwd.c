#include "../minishell.h"

char	*pwd(void)
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
	g_term.last_exit = 0;
	return (path);
}
