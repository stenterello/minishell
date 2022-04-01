#include "minishell.h"

void	die(char *msg)
{
	ft_putendl_fd(msg, 2);
	exit(-1);
}

int	is_abs_path(char *line)
{
	int	i;

	i = 2;
	while (line[i] == 32 || (line[i] > 8 && line[i] < 14))
		i++;
	if (line[i] == '/')
		return (1);
	return (0);
}

char	*get_path(char *line)
{
	char	*ret;
	int		i;

	i = 2;
	while (line[i] == 32 || (line[i] > 8 && line[i] < 14))
		i++;
	ret = malloc(sizeof(char) * (ft_strlen(line) - i + 1));
	if (!ret)
		die("Malloc error trying to get absolute path [CD]");
	ft_strlcpy(ret, &line[i], ft_strlen(line) - i + 1);
	return (ret);
}
