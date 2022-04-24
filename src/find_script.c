#include "minishell.h"

void	get_full_path(char *dir_name, t_command *cmd)
{
	char	*tmp;

	tmp = NULL;
	malloc_and_check_char(&tmp, ft_strlen(dir_name) + ft_strlen(cmd->cmd) + 2);
	ft_strlcpy(tmp, dir_name, ft_strlen(dir_name) + 1);
	ft_strlcat(tmp, "/", ft_strlen(dir_name) + 2);
	ft_strlcat(tmp, cmd->cmd, ft_strlen(dir_name) + ft_strlen(cmd->cmd) + 2);
	free(cmd->cmd);
	malloc_and_check_char(&cmd->cmd, ft_strlen(tmp) + 1);
	ft_strlcpy(cmd->cmd, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

int	search_in_dir(DIR *stream, t_command *cmd, char *dir_name)
{
	struct dirent	*entry;

	entry = readdir(stream);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, cmd->cmd, (ft_strlen(cmd->cmd) + 1)))
		{
			get_full_path(dir_name, cmd);
			if (!access(cmd->cmd, X_OK))
			{
				born_child(cmd);
				return (1);
			}
		}
		entry = readdir(stream);
	}
	return (0);
}

int	find_script(t_command *cmd)
{
	char	**path;
	DIR		*stream;
	int		is_exec;
	int		i;

	path = ft_split(ft_getenv("PATH\0"), ':');
	i = 0;
	if (path)
	{
		while (path[i])
		{
			if (ft_strlen(path[i]) > 255)
				die("Path name too long");
			stream = opendir(path[i]);
			// Quando la shell cerca nelle cartelle del PATH
			// non restituisce errore se la cartella non può essere letta
			// Lasciare o no questo avviso?
			if (stream == NULL)
				die("Error opening directory");
			is_exec = search_in_dir(stream, cmd, path[i]);
			if (is_exec)
			{
				closedir(stream);
				free_array_of_array(path);
				return (0);
			}
			closedir(stream);
			i++;
		}
		free_array_of_array(path);
	}
	return (-1);
}
