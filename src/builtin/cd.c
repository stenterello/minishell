#include "../minishell.h"

void	cd(t_command *cmd, t_term *term)
{
	char	*dest;
	int		ret;

	if (!cmd->args[1])
	{
		ret = chdir(getenv("HOME"));
		if (ret == -1)
		{
			printf("%s\n", strerror(errno));
			term->last_exit = 1;
		}
		else
			term->last_exit = 0;
	}
	else if (cmd->args[2])
	{
		ft_putstr_fd(cmd->cmd, cmd->stderr);
		ft_putendl_fd(": too many arguments", cmd->stderr);
		term->last_exit = 1;
	}
	else
	{
		dest = get_path(cmd->args[1]);
		ret = chdir(dest);
		if (ret == -1)
		{
			printf("%s: %s\n", dest, strerror(errno));
			term->last_exit = 1;
		}
		else
			term->last_exit = 0;
		free(dest);
	}
}
