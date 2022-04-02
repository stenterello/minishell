#include "minishell.h"

void	elaborate_cmd(char *line)
{
	char	*ret;
	char	*ptr;
	char	**args;
	int		i;

	i = 0;
	args = NULL;
	ptr = NULL;
	ret = malloc(sizeof(char) * (ft_strlen(line) + 1));
	if (!ret)
		die ("Malloc error");
	ft_strlcpy(ret, line, ft_strlen(line) + 1);
	ptr = ft_strchr(ret, '$');
	while (ptr != NULL)
	{
		if (!single_quoted(ptr, dollar_pos(ret)))
			line = get_variable(line);
		ptr = ft_strchr(line, '$');
		if (ptr != NULL)
			ptr++;
	}
	free(ret);
	args = ft_split(line, ' ');
	if (!ft_strncmp(args[0], "pwd\0", 4))
	{
		ret = pwd();
		ft_putendl_fd(ret, 1);
		free(ret);
	}
	else if (!ft_strncmp(args[0], "exit\0", 5))
		exit(0);
	else if (!ft_strncmp(args[0], "cd\0", 3))
		cd(args);
	else if (!ft_strncmp(args[0], "echo\0", 5))
		echo(args);
	else
	{
		if (find_script(args) == -1)
			cmd_not_found(line);
	}
	while (args[i])
		free(args[i++]);
	free(args);
}
