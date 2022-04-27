#include "minishell.h"

int	sup_builtin(t_command *cmd)
{
	if (!ft_strncmp(cmd->cmd, "cd\0", 3))
		cd(cmd);
	else if (!ft_strncmp(cmd->cmd, "echo\0", 5))
		echo(cmd);
	else if (!ft_strncmp(cmd->cmd, "exit\0", 5))
		exit_cmd(cmd);
	else if (!ft_strncmp(cmd->cmd, "env\0", 4))
	{
		if (cmd->to_pipe)
			define_pipe(cmd);
		if (cmd->to_pipe_to)
			define_pipe_to(cmd);
		env();
	}
	else if (!ft_strncmp(cmd->cmd, "export\0", 7))
		export(cmd);
	else if (!ft_strncmp(cmd->cmd, "unset\0", 6))
		unset(cmd);
	else
		return (0);
	return (1);
}

int	builtin(t_command *cmd)
{
	char	*ret;
	int		sup;

	sup = 0;
	if (!cmd->cmd)
		return (0);
	if (!ft_strncmp(cmd->cmd, "pwd\0", 4))
	{
		ret = pwd();
		if (cmd->to_pipe)
			define_pipe(cmd);
		if (cmd->to_pipe_to)
			define_pipe_to(cmd);
		ft_putendl_fd(ret, 1);
		free(ret);
	}
	sup = sup_builtin(cmd);
	if (cmd->to_pipe || cmd->to_pipe_to || cmd->redir_in || cmd->redir_out)
		restore_fd(cmd);
	g_term.child = 0;
	return (sup);
}

int	ft_isupper(char c)
{
	if (c >= 65 && c <= 90)
		return (1);
	return (0);
}
