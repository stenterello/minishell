#include "minishell.h"

int	ft_islower(char c)
{
	if (97 <= c && c <= 122)
		return (1);
	return (0);
}

int	is_wildcard(char c)
{
	if (c == '*' || c == '?' || c == '[')
		return (1);
	return (0);
}

int	is_in(char *range, char c)
{
	int	i;

	i = 0;
	while (range[i])
	{
		if (c == range[i])
			return (1);
		i++;
	}
	return (0);
}

int	is_verified_brackets(char *brackets, char file_char)
{
	char	*rules;

	if (count_letters(brackets) == -1)
		return (-1);
	rules = get_letters(brackets);
	if (is_in(rules, file_char))
	{
		free(rules);
		return (1);
	}
	free(rules);
	return (0);
}

int	has_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '?' || str[i] == '*' || str[i] == '[') && !is_open(str, i))
			return (1);
		i++;
	}
	return (0);
}

int	is_verified(char *file, char **portions)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (portions[i] && file[j])
	{
		while (portions[i] && portions[i][0] == '*')
			i++;
		while (portions[i] && portions[i][0] == '?')
		{
			i++;
			j++;
			if (!file[j])
				return (0);
		}
		if (portions[i] && portions[i - 1][0] == '*' && portions[i][0] == '[' && file[j])
		{
			while (portions[i] && portions[i][0] == '[' && file[j])
			{
				while (file[j] && portions[i] && is_verified_brackets(portions[i], file[j]) == 0)
					j++;
				if (is_verified_brackets(portions[i], file[j]) == -1)
					return (-1);
				if (is_verified_brackets(portions[i], file[j]) == 1)
				{
					i++;
					j++;
				}
				else if (!file[j])
					return (0);
			}
		}
		else if (portions[i] && portions[i][0] == '[' && file[j])
		{
			if (is_verified_brackets(portions[i], file[j]) == -1)
				return (-1);
			if (is_verified_brackets(portions[i], file[j]) == 1)
			{
				i++;
				j++;
			}
			else
				return (0);
		}
		if (portions[i] && portions[i - 1][0] == '*' && ft_isalnum(portions[i][0]))
		{
			while (file[j] && ft_strncmp(&file[j], portions[i], ft_strlen(portions[i])))
				j++;
			if (!file[j])
				return (0);
			i++;
		}
		else
		{
			if (portions[i] && ft_isalnum(portions[i][0]) && !ft_strncmp(&file[j], portions[i], ft_strlen(portions[i])))
			{
				j += ft_strlen(portions[i]);
				i++;
			}
			else if (portions[i] && ft_isalnum(portions[i][0]))
				return (0);
		}
	}
	return (1);
}
