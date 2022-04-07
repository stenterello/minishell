#include "minishell.h"

// int	follow_path(char **args)
// {
// 	ret = execve(args[0], &args, NULL);

// }

// int	follow_path(char **args)
// {
// 	char	**splitted;
// 	char	*path;
// 	int		i;
// 	int		ret;

// 	splitted = ft_split(args[0], '/');
// 	i = 0;
// 	while (splitted[i] && splitted[i + 1])
// 	{
// 		path = malloc(sizeof(char) * (ft_strlen(splitted[i]) + 2));
// 		if (!path)
// 			die("Malloc error");
// 		if (!ft_strncmp(args[0], "/", 1) && i == 0)
// 		{
// 			ft_strlcpy(path, "/", 2);
// 			ft_strlcpy(&path[1], splitted[i], ft_strlen(splitted[i]) + 2);
// 		}
// 		else
// 			ft_strlcpy(path, splitted[i], ft_strlen(splitted[i]) + 1);
// 		ret = chdir(path);
// 		free(path);
// 		if (ret == -1)
// 			return (ret);
// 		i++;
// 	}
// 	// i = 0;
// 	// while (splitted[i] && splitted[i + 1])
// 	// {
// 	// 	ret = chdir(splitted[i]);
// 	// 	if (ret == -1)
// 	// 		return (ret);
// 	// 	i++;
// 	// }
// 	ret = execve(splitted[i], &splitted[i], NULL);
// 	return (ret);
// }

void	execute(t_input *input)
{
	//t_command	command;
	int			i;

	//init_command(&command);
	//parse(input, &command);
	if (builtin(input))
	{
		i = 0;
		while (input->args[i])
			free(input->args[i++]);
		free(input->args);
		return ;
	}
	else
	{
		if (ft_strchr(input->args[0], '/') == NULL)
		{
			if (find_script(input->args) == -1)
				cmd_not_found(input->line);
		}
		else
		{
			if (execve(input->args[0], input->args, NULL) == -1)
				cmd_not_found(input->line);
		}
		i = 0;
		while (input->args[i])
			free(input->args[i++]);
		free(input->args);
	}
	
}
