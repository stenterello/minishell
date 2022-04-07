#include "../minishell.h"

void	cd(char **args)
{
	char	*dest;
	int		ret;

	if (!args[1])
	{
		ret = chdir(getenv("HOME"));
		if (ret == -1)
		{
			printf("%s\n", strerror(errno));
			return ;
		}
	}
	else
	{
		dest = get_path(args[1]);
		ret = chdir(dest);
		if (ret == -1)
		{
			printf("%s: %s\n", dest, strerror(errno));
			return ;
		}
		free(dest);
	}
}
