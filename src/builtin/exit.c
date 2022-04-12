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

void	exit_cmd(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i] != NULL)
		i++;
	if (i > 2)
	{
		ft_putendl_fd("exit: too many arguments", cmd->stderr);
		return ;
	}
	else if (i == 2 && !is_number(cmd->args[1]))
	{
		ft_putendl_fd("exit: numeric argument required", cmd->stderr);
		return ;
	}
	if (i == 1)
		exit(0);
	else
		exit(ft_atoi(cmd->args[i - 1]));
}
