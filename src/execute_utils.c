#include "minishell.h"

char	*get_full_path(char *dir_name, char *name)
{
	char	*ret;

	ret = malloc(sizeof(char) * (ft_strlen(dir_name) + ft_strlen(name) + 2));
	if (!ret)
		die("Malloc error");
	ft_strlcpy(ret, dir_name, ft_strlen(dir_name) + 1);
	ft_strlcat(ret, "/", ft_strlen(dir_name) + 2);
	ft_strlcat(ret, name, ft_strlen(dir_name) + ft_strlen(name) + 2);
	free(name);
	return (ret);
}

void	cmd_not_found(char *line)
{
	char	*tmp;
	int		i;

	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	tmp = malloc(sizeof(char) * (i + 1));
	ft_strlcpy(tmp, line, i + 1);
	ft_putstr_fd(tmp, 2);
	ft_putendl_fd(": command not found", 2);
	free(tmp);
	exit(127);
}

int	search_in_dir(DIR *stream, t_command *cmd, char *dir_name)
{
	struct dirent	*entry;
	pid_t			child;
	int				status;

	entry = readdir(stream);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, cmd->cmd, (ft_strlen(cmd->cmd) + 1)))
		{
			cmd->cmd = get_full_path(dir_name, cmd->cmd);
			if (!access(cmd->cmd, X_OK))
			{
				child = fork();
				if (child == -1)
					die("Error while forking");
				if (child == 0)
					execve(cmd->cmd, cmd->args, NULL);
				else
					waitpid(-1, &status, 0);
				return (1);
			}
			else
				return (2);
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

	path = ft_split(getenv("PATH"), ':');
	i = 0;
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
		if (is_exec == 2)
		{
			printf("%s\n", strerror(errno));
			return (2);
		}
		else if (is_exec == 1)
		{
			closedir(stream);
			i = 0;
			while (path[i])
				free(path[i++]);
			free(path);
			return (0);
		}
		closedir(stream);
		i++;
	}
	i = 0;
	while (path[i])
		free(path[i++]);
	free(path);
	return (-1);
}
