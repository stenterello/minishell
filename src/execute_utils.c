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

void	cmd_not_found(t_command *cmd)
{
	ft_putstr_fd(cmd->cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	g_term.last_exit = 127;
}

int	search_in_dir(DIR *stream, t_command *cmd, char *dir_name)
{
	struct dirent	*entry;
	int				status;

	entry = readdir(stream);
	status = 0;
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, cmd->cmd, (ft_strlen(cmd->cmd) + 1)))
		{
			get_full_path(dir_name, cmd);
			if (!access(cmd->cmd, X_OK))
			{
				if (cmd->to_pipe)
					define_pipe(cmd);
				if (cmd->to_pipe_to)
					define_pipe_to(cmd);
				g_term.child = fork();
				if (g_term.child == -1)
					die("Error while forking");
				if (g_term.child == 0)
				{
					add_signals();
					free_dict(g_term.var);
					execve(cmd->cmd, cmd->args, NULL);
				}
				else
				{
					close(cmd->piped_fd);
					wait3(g_term.child, &status, 0);
				}
				if (WIFEXITED(status))
					g_term.last_exit = status / 256;
				else
					g_term.last_exit = status;
				g_term.child = -1;
				restore_fd(cmd);
				return (1);
			}
		}
		entry = readdir(stream);
	}
	return (0);
}

char	*ft_getenv(char *line)
{
	t_dict	*tmp;
	t_dict	*tmp2;

	if (g_term.env)
	{
		tmp = g_term.env;
		while (tmp)
		{
			if (!ft_strncmp(tmp->key, line, ft_strlen(line) + 1))
				return (tmp->value);
			tmp = tmp->next;
		}
	}
	if (g_term.var && g_term.var->key)
	{
		tmp2 = g_term.var;
		while (tmp2)
		{
			if (!ft_strncmp(tmp2->key, line, ft_strlen(line) + 1))
				return (tmp2->value);
			tmp2 = tmp2->next;
		}
	}
	return (NULL);
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
	}
	return (-1);
}
