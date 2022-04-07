#include "../minishell.h"

void	exit_cmd(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	if (i > 2)
	{
		ft_putendl_fd(": too many arguments", 2);
		return ;
	}
	if (i == 1)
		exit(0);
	else
		exit(ft_atoi(args[i - 1]));
}
