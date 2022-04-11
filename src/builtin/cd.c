#include "../minishell.h"

void	cd(t_command *cmd)
{
	char	*dest;
	int		ret;

	if (!cmd->args[1])
	{
		ret = chdir(getenv("HOME"));
		if (ret == -1)
		{
			printf("%s\n", strerror(errno));
			return ;
		}
	}
	else if (cmd->args[2])
	{
		ft_putstr_fd(cmd->cmd, 2);
		ft_putchar_fd(' ', 2);
		ft_putendl_fd(": too many arguments", 2);
	}
	else
	{
		dest = get_path(cmd->args[1]);
		ret = chdir(dest);
		if (ret == -1)
		{
			printf("%s: %s\n", dest, strerror(errno));
			return ;
		}
		free(dest);
	}
}
