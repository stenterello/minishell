#include "../minishell.h"

void	cd_home(char *line)
{
	char	**act_path;
	char	**dest;
	int		i;

	i = 0;
	act_path = ft_split(pwd(), '/');
	dest = ft_split(line, '/');
	while (act_path[i])
		printf("%s\n", act_path[i++]);
	i = 0;
	while (dest[i])
		printf("%s\n", dest[i++]);
	
}
