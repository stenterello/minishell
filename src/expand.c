#include "minishell.h"

int	dollar_pos(char *line)
{
	int	ret;

	ret = 0;
	while (line[ret] != '$')
		ret++;
	return (ret);
}

int	var_name_len(char *line, int j)
{
	while (ft_isupper(line[j]))
		j++;
	return (j);
}

int	single_quoted(char *line, int ind)
{
	int	i;
	int	s_quoted;

	i = 0;
	s_quoted = 0;
	while (line[i] && i < ind)
	{
		if (line[i] == '\'' && !s_quoted)
			s_quoted = 1;
		if (line[i] == '$' && !s_quoted)
			return (0);
		if (line[i] == '\'' && s_quoted)
			s_quoted = 0;
		i++;
	}
	return (0);
}

char	*get_variable(char *line)
{
	int		i;
	int		j;
	char	*var_name;
	char	*value;
	char	*ret;

	i = dollar_pos(line) + 1;
	j = var_name_len(line, i);
	var_name = malloc(sizeof(char) * (j - i + 1));
	if (!var_name)
		die("Malloc error");
	ft_strlcpy(var_name, &line[i], j - i + 1);
	value = getenv(var_name);
	i = dollar_pos(line);
	ret = malloc(sizeof(char) * (ft_strlen(value) + (ft_strlen(line) - j) + 2));
	if (!ret)
		die("Malloc error");
	ft_strlcpy(ret, line, i + 1);
	ft_strlcpy(&ret[i], value, ft_strlen(value) + 1);
	if (line[i + ft_strlen(var_name) + 1])
		ft_strlcpy(&ret[i + ft_strlen(value)], &line[i + ft_strlen(var_name) + 1], ft_strlen(ret) + ft_strlen(value));
	free(var_name);
	free(line);
	return (ret);
}
