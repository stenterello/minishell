#include "../minishell.h"

void	cd(t_command *cmd)
{
	char	*dest;
	int		ret;

	ret = 0;
	if (cmd->args[1] && !ft_strncmp(cmd->args[1], "-\0", 2))
	{
		ret = chdir(ft_getenv("OLDPWD"));
		if (ret == -1)
		{
			ft_putstr_fd(last_field(ft_getenv("SHELL")), 2);
			ft_putendl_fd(": cd: no OLDPWD in variables", 2);
			g_term.last_exit = 1;
			return ;
		}
	}
	else if (!cmd->args[1])
	{
		ret = chdir(ft_getenv("HOME"));
		if (ret == -1)
		{
			//ft_putendl_fd("No such file or directory", 2);
			printf("%s\n", strerror(errno));
			g_term.last_exit = 1;
		}
		else
			g_term.last_exit = 0;
	}
	else if (cmd->args[2])
	{
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putendl_fd(": too many arguments", STDERR_FILENO);
		g_term.last_exit = 1;
	}
	else
	{
		dest = get_path(cmd->args[1]);
		ret = chdir(dest);
		if (ret == -1)
		{
			printf("%s: %s\n", dest, strerror(errno));
			g_term.last_exit = 1;
		}
		else
			g_term.last_exit = 0;
		free(dest);
	}
}
