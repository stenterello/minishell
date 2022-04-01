#include "../minishell.h"

void	cd(char *line)
{
	char	*dest;
	int		ret;

	if (ft_strlen(line) == 2)
	{
		ret = chdir(getenv("HOME"));
		if (ret != -1)
			die("cd error going home");
	}
	else
	{
		dest = get_path(line);
		ret = chdir(dest);
		if (ret == -1)
		{
			ft_putendl_fd("no such file/directory, change my message, thanks", 2);
			return ;
		}
		free(dest);
	}
}
