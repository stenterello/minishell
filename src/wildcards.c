#include "minishell.h"

int	is_wildcard(char c)
{
	if (c == '*' || c == '?' || c == '[')
		return (1);
	return (0);
}

int	count_portions(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (line[i])
	{
		if (line[i] == '[' && !is_open(line, i))
		{
			ret++;
			while (line[i] && line[i] != ']')
				i++;
			if (is_open(line, i))
				return (-1);
		}
		else if (!is_open(line, i) && is_wildcard(line[i]))
		{
			ret++;
		}
		else if (ft_isalnum(line[i]))
		{
			ret++;
			while (line[i] && !is_wildcard(line[i]))
				i++;
			i--;
		}
		i++;
	}
	return (ret);
}

int	try_parse_brackets(char *line)
{
	int		i;
	int		flag;

	i = 0;
	if (line[i] == '[')
		i++;
	else
		return (-1);
	while (ft_isalnum(line[i]))
	{
		if (ft_isalpha(line[i]))
			flag = 0;
		else if (ft_isdigit(line[i]))
			flag = 1;
		if (line[++i] != '-')
			return (-1);
		if (ft_isalpha(line[++i]) && flag)
			return (-1);
		else if (ft_isdigit(line[i]) && !flag)
			return (-1);
		else if (!ft_isalnum(line[i]))
			return (-1);
		i++;
	}
	if (line[i] != ']')
		return (-1);
	return (i + 1);
}

void	insert_brackets_param(char *line, char *portion, int len)
{
	int		i;
	char	*tmp;

	i = 0;
	malloc_c(&tmp, len + 1);
	while (ft_isalnum(line[i]) || line[i] == '[' || line[i] == '-')
	{
		tmp[i] = line[i];
		i++;
	}
	tmp[i++] = ']';
	tmp[i] = '\0';
	ft_strlcpy(portion, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

int	take_brackets_param(char *line, char **portion)
{
	if (try_parse_brackets(line) == -1)
		return (-1);
	else if (try_parse_brackets(line))
	{
		malloc_c(portion, try_parse_brackets(line) + 1);
		insert_brackets_param(line, *portion, try_parse_brackets(line));
	}
	return (1);
}

void	take_string_portion(char *line, char **portion)
{
	int	i;

	i = 0;
	while (line[i] && !is_wildcard(line[i]))
		i++;
	malloc_c(portion, i + 1);
	ft_strlcpy(*portion, line, i + 1);
}

int	is_verified(char *file, char **portions)
{
	int	i;

	i = 0;
	while (portions[i] && file[i])
	{
		
	}
}

int	count_results(char **portions)
{
	DIR				*stream;
	int				ret;
	struct dirent	*entry;
	struct stat		file_stat;

	ret = 0;
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	entry = readdir(stream);
	while (entry)
	{
		stat(entry->d_name, &file_stat);
		if ((file_stat.st_mode & S_IFMT) != S_IFDIR)
		{
			if (is_verified(entry->d_name, portions))
				ret++;
		}
		entry = readdir(stream);
	}
	closedir(stream);
	return (ret);
	// Crea stream
	// per ogni entry della cartella
	// verifica che tutto rispetti le indicazioni
	// ritorna il numero di risultati
}

int	wild_search(char **portions, t_command *cmd)
{
	char	**results;

	malloc_c_ptr(&results, count_results(portions));
}

int	guess(t_command *cmd, int i)
{
	int		j;
	int		k;
	char	**portions;

	j = 0;
	k = 0;
	malloc_c_ptr(&portions, count_portions(cmd->args[i]) + 1);
	while (cmd->args[i][j])
	{
		if (!is_open(cmd->args[i], j) && (cmd->args[i][j] == '*' || cmd->args[i][j] == '?'))
		{
			malloc_c(&portions[k], 2);
			ft_strlcpy(portions[k], &cmd->args[i][j], 2);
			k++;
		}
		else if (!is_open(cmd->args[i], j) && cmd->args[i][j] == '[')
		{
			if (take_brackets_param(&cmd->args[i][j], &portions[k]) == -1)
				return (-1);
			k++;
			j += try_parse_brackets(&cmd->args[i][j]) - 1;
		}
		else
		{
			take_string_portion(&cmd->args[i][j], &portions[k]);
			j += ft_strlen(portions[k++]) - 1;
		}
		j++;
	}
	portions[k] = NULL;
	if (wild_search(portions, cmd) == -1)
		return (-1);
	free_array_of_array(portions);
	return (1);
}

int	check_wildcards(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '*') != NULL
			|| ft_strchr(cmd->args[i], '[') != NULL
			|| ft_strchr(cmd->args[i], '?') != NULL)
		{
			if (guess(cmd, i) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
