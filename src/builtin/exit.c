#include "../minishell.h"

int	is_number(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isdigit(line[i]))
			return (0);
		i++;
	}
	return (1);
}

int	sup_exit_cmd(t_command *cmd, int i)
{
	if (i > 2)
	{
		ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
		return (0);
	}
	else if (i == 2 && !is_number(cmd->args[1]))
	{
		ft_putendl_fd("exit: numeric argument required", STDERR_FILENO);
		return (0);
	}
	return (1);
}

void	exit_cmd(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i] != NULL)
		i++;
	if (!sup_exit_cmd(cmd, i))
		return ;
	reset_term();
	ft_putendl_fd("exit", STDOUT_FILENO);
	i = 0;
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	free(cmd->cmd);
	free_dict(g_term.env);
	free_dict(g_term.var);
	free(g_term.input.line);
	if (i == 1)
		exit(0);
	else
		exit(ft_atoi(cmd->args[i - 1]));
}
