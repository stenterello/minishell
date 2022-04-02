#include "../minishell.h"

void	echo(char **args)
{
	int	i;

	i = 1;
	if (!args[1])
		ft_putendl_fd(NULL, 1);
	else
	{
		while (args[i])
		{
			ft_putstr_fd(args[i++], 1);
			if (args[i])
				ft_putchar_fd(' ', 1);
		}
		ft_putchar_fd('\n', 1);
	}
}
