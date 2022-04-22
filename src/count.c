#include "minishell.h"

int	count_params(char *line)
{
	int	count;
	int	i;
	int	s_quot;
	int	d_quot;

	count = 1;
	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (line[i])
	{
		if (line[i] == ' ' && !d_quot && !s_quot)
			count++;
		else if (line[i] == '"' && !s_quot && !d_quot)
			d_quot = 1;
		else if (line[i] == '"' && !s_quot && d_quot)
			d_quot = 0;
		else if (line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		i++;
	}
	return (count);
}

int	count_args(char **tmp)
{
	int	i;

	i = 0;
	while (tmp[i] && ft_strncmp(tmp[i], "<\0", 2) && ft_strncmp(tmp[i], "<<\0", 3) && ft_strncmp(tmp[i], ">\0", 2) && ft_strncmp(tmp[i], ">>\0", 3) && ft_strncmp(tmp[i], "|\0", 2))
		i++;
	return (i);
}

int	next_arg_len(char *line)
{
	int	i;

	i = 0;
	while ((line[i] && line[i] != ' ' && line[i] != '>' && line[i] != '<' && line[i] != '|' && !is_open(line, i)) || (line[i] && is_open(line, i)))
		i++;
	if (line[0] == '<' || line[0] == '>')
		return (ft_strlen(line));
	return (i);
}
