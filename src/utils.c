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

char	*get_full_path(char *dir_name, char *name)
{
	char	*ret;

	ret = malloc(sizeof(char) * (ft_strlen(dir_name) + ft_strlen(name) + 2));
	if (!ret)
		die("Malloc error");
	ft_strlcpy(ret, dir_name, ft_strlen(dir_name) + 1);
	ft_strlcat(ret, "/", ft_strlen(dir_name) + 2);
	ft_strlcat(ret, name, ft_strlen(dir_name) + ft_strlen(name) + 2);
	return (ret);
}

int	search_in_dir(DIR *stream, char *line, char *dir_name)
{
	struct dirent	*entry;
	char			*args[2];
	pid_t			child;
	int				status;

	entry = readdir(stream);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, line, (ft_strlen(line) + 1)))
		{
			args[0] = get_full_path(dir_name, line);
			if (!access(args[0], X_OK))
			{
				args[1] = NULL;
				child = fork();
				if (child == -1)
					die("Error while forking");
				if (child == 0)
				{
					execve(args[0], args, NULL);
				}
				else
					waitpid(-1, &status, 0);
				free(args[0]);
				return (1);
			}
			else
				return (2);
		}
		entry = readdir(stream);
	}
	return (0);
}

int	find_script(char *line)
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
		is_exec = search_in_dir(stream, line, path[i]);
		if (is_exec == 2)
		{
			printf("%s\n", strerror(errno));
			return (2);
		}
		else if (is_exec == 1)
		{
			closedir(stream);
			return (0);
		}
		closedir(stream);
		i++;
	}
	return (-1);
}
