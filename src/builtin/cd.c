#include "../minishell.h"

void	sup_cd1(int ret, t_command *old, char *act)
{
	ret = chdir(ft_getenv("OLDPWD"));
	if (ret == -1)
	{
		ft_putstr_fd(last_field(ft_getenv("SHELL")), 2);
		ft_putendl_fd(": cd: no OLDPWD in variables", 2);
		g_term.last_exit = 1;
		return ;
	}
	else
		sup_sup_cd1(old, act);
}

void	sup_cd2(int ret, t_command *old, char *act)
{
	ret = chdir(ft_getenv("HOME"));
	if (ret == -1)
	{
		printf("%s\n", strerror(errno));
		g_term.last_exit = 1;
	}
	else
		sup_sup_cd2(old, act);
}

void	sup_cd3(t_command *cmd, char *act, t_command *old)
{
	char		*dest;
	int			ret;

	dest = get_path(cmd->args[1]);
	ret = chdir(dest);
	if (ret == -1)
	{
		printf("%s: %s\n", dest, strerror(errno));
		g_term.last_exit = 1;
	}
	else
		sup_sup_cd3(act, old);
	free(dest);
}

void	cd(t_command *cmd)
{
	int			ret;
	char		*act;
	t_command	*old;

	ret = 0;
	act = pwd();
	old = malloc(sizeof(t_command));
	if (!old)
		die("Malloc error");
	old->args = NULL;
	if (cmd->args[1] && !ft_strncmp(cmd->args[1], "-\0", 2))
		sup_cd1(ret, old, act);
	else if (!cmd->args[1])
		sup_cd2(ret, old, act);
	else if (cmd->args[2])
	{
		printf("%s: %s: %s\n",
			last_field(ft_getenv("SHELL")), cmd->cmd, strerror(errno));
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putendl_fd(": too many arguments", STDERR_FILENO);
		g_term.last_exit = 1;
	}
	else
		sup_cd3(cmd, act, old);
}
