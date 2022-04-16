#include "minishell.h"

int	delimiter_len(char *line)
{
	int	i;

	i = 0;
	while (ft_isalnum(line[i]))
		i++;
	return (i);
}

char	*take_delimiter(char *line)
{
	int		i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (ft_strncmp(&line[i], " <<", 3))
		i++;
	i += 4;
	while (!ft_isalnum(line[i]))
		i++;
	malloc_and_check_char(&ret, delimiter_len(&line[i]) + 1);
	ft_strlcpy(ret, &line[i], delimiter_len(&line[i]) + 1);
	return (ret);
}

int	to_continue(char *typed, char *delimiter)
{
	int	i;

	i = ft_strlen(typed) - ft_strlen(delimiter);
	if (!ft_strncmp(&typed[i], delimiter, ft_strlen(delimiter) + 1))
		return (0);
	return (1);
}

void	clean_heredoc(char *line, char *bench)
{
	int	i;

	i = 0;
	while (ft_strncmp(&line[i], bench, ft_strlen(bench)) && i <= (int)(ft_strlen(line) - ft_strlen(bench)))
		i++;
	line[i] = '\0';
}

char	*to_string(char *line)
{
	int		i;
	char	*ret;

	ret = NULL;
	malloc_and_check_char(&ret, ft_strlen(line) + 3);
	i = 0;
	while (line[i] != ' ')
		i++;
	i++;
	ft_strlcpy(ret, line, i + 1);
	ft_strlcat(ret, "\"", ft_strlen(ret) + 2);
	ft_strlcat(ret, &line[i + 1], ft_strlen(&line[i]) + ft_strlen(ret) + 1);
	ft_strlcat(ret, "\"", ft_strlen(ret) + 2);
	return (ret);
}

void	write_to_stdin(char *line)
{
	int	i;

	i = 0;
	while (line[i])
		ft_putchar_fd(line[i++], 1);
}
