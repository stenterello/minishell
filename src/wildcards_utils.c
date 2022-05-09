#include "minishell.h"

int	try_parse_brackets(char *line)
{
	int		i;

	i = 0;
	if (line[i] == '[')
		i++;
	else
		return (-1);
	while (ft_isascii(line[i]) && line[i] != ']' && line[i])
		i++;
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
	while (line[i] && ft_isascii(line[i]) && line[i] != ']')
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
	if (try_parse_brackets(line) != -1)
	{
		malloc_c(portion, try_parse_brackets(line) + 1);
		insert_brackets_param(line, *portion, try_parse_brackets(line));
		return (1);
	}
	return (-1);
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

char	*chrs_range(char a, char b)
{
	char	*tmp;
	int		i;
	int		j;

	i = a + 1;
	j = 0;
	tmp = malloc(sizeof(char) * (b - a + 1));
	while (i < b)
		tmp[j++] = (char)i++;
	tmp[j] = '\0';
	return (tmp);
}

char	*get_letters(char *brackets)
{
	int		i;
	int		len;
	char	*ret;
	char	*tmp;

	i = 1;
	len = 2;
	ret = malloc(sizeof(char) * count_letters(brackets) + 1);
	while (brackets[i] != ']')
	{
		if (brackets[i] != '-')
		{
			if (i == 1)
				ft_strlcpy(ret, &brackets[i], len++);
			else
				ft_strlcat(ret, &brackets[i], len++);
		}
		else
		{
			tmp = chrs_range(brackets[i - 1], brackets[i + 1]);
			len += ft_strlen(tmp) - 1;
			ft_strlcat(ret, tmp, len++);
			free(tmp);
		}
		i++;
	}
	return (ret);
}

char	**get_results(char **portions, int len)
{
	DIR				*stream;
	char			**ret;
	int				i;
	struct dirent	*entry;

	i = 0;
	malloc_c_ptr(&ret, len + 1);
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (NULL);
	}
	entry = readdir(stream);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, ".", 1) && is_verified(entry->d_name, portions) > 0)
		{
			malloc_c(&ret[i], ft_strlen(entry->d_name) + 1);
			ft_strlcpy(ret[i++], entry->d_name, ft_strlen(entry->d_name) + 1);
		}
		entry = readdir(stream);
	}
	ret[i] = NULL;
	closedir(stream);
	return (ret);
}
