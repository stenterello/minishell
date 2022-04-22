#include "minishell.h"

static int	count_fields(char *s, char c)
{
	int	i;
	int	fields;
	int	flag;

	i = 0;
	fields = 0;
	flag = 1;
	while (*(s + i) != '\0')
	{
		if (*(s + i) != c && flag == 1 && !is_open(s, i))
		{
			fields++;
			flag = 0;
		}
		if (*(s + i) == c && !is_open(s, i))
			flag = 1;
		i++;
	}
	return (fields);
}

static int	count_len(char *s, char c)
{
	int	i;

	i = 0;
	while (*(s + i))
	{
		if (*(s + i) == c && !is_open(s, i))
			return (i);
		i++;
	}
	return (i);
}

static int	find_start(char *s, char c)
{
	int	i;

	i = 0;
	while (*(s + i) == c && !is_open(s, i))
		i++;
	return (i);
}

static int	find_next_start(char *s, char c)
{
	int	i;

	i = 0;
	while (*(s + i))
	{
		if (*(s + i) == c && !is_open(s, i))
			break ;
		i++;
	}
	i += find_start(&s[i], c);
	return (i);
}

char	**split_fields(char *s, char c)
{
	int		i;
	int		start;
	int		fields;
	char	**ret;

	if (!s)
		return (NULL);
	fields = count_fields(s, c);
	ret = malloc(sizeof(char *) * (fields + 1));
	if (!ret)
		return (NULL);
	i = 0;
	start = find_start(s, c);
	while (i < fields)
	{
		ret[i] = malloc(sizeof(char) * count_len(&s[start], c) + 1);
		if (!ret[i])
			return (NULL);
		ft_strlcpy(ret[i], &s[start], count_len(&s[start], c) + 1);
		if (ret[i][0] == '\"')
			ret[i] = ft_strtrim(ret[i], "\"");
		else if (ret[i][0] == '\'')
			ret[i] = ft_strtrim(ret[i], "\'");
		start += find_next_start(&s[start], c);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
