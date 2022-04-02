#include "minishell.h"

int	go_nl(char *line)
{
	int	i;
	int	s_quoted;
	int	d_quoted;

	i = 0;
	s_quoted = 0;
	d_quoted = 0;
	while (line[i])
	{
		if (!s_quoted && !d_quoted && line[i] == '\'')
			s_quoted = 1;
		else if (s_quoted && line[i] == '\'')
			s_quoted = 0;
		else if (!d_quoted && !s_quoted && line[i] == '"')
			d_quoted = 1;
		else if (d_quoted && line[i] == '"')
			d_quoted = 0;
		i++;
	}
	if (line[i - 1] == '\\')
	{
		line[i - 1] = 0;
		return (1);
	}
	if (s_quoted || d_quoted)
	{
		line[i] = '\n';
		return (1);
	}
	return (0);
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

	i = 0;
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
	free(name);
	return (ret);
}

void	cmd_not_found(char *line)
{
	ft_putstr_fd(line, 2);
	ft_putendl_fd(": command not found", 2);
}
