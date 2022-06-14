/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_count.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 11:01:53 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/10 16:06:01 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sup_count(char *line, int i, int ret)
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
	return (ret);
}

int	count_portions(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (line[i])
	{
		ret = sup_count(line, i, ret);
		i++;
	}
	return (ret);
}

int	count_range(char a, char b)
{
	int	i;

	i = a + 1;
	while (i < b)
		i++;
	return (i - a + 1);
}

int	count_letters(char *brackets)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (brackets[i] != ']')
	{
		if (brackets[i] == '-'
			&& count_range(brackets[i - 1], brackets[i + 1]) != -1)
			ret += count_range(brackets[i - 1], brackets[i + 1]);
		else if (count_range(brackets[i - 1], brackets[i + 1]) == -1)
			return (-1);
		i++;
		if (ft_strchr("!^-]", brackets[i]) == NULL)
			ret++;
	}
	return (ret);
}

int	count_results(char **portions)
{
	DIR				*stream;
	int				ret;
	struct dirent	*entry;

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
		if (ft_strncmp(entry->d_name, ".", 1)
			&& is_verified(entry->d_name, portions) > 0)
			ret++;
		else if (is_verified(entry->d_name, portions) == -1)
			return (-1);
		entry = readdir(stream);
	}
	closedir(stream);
	return (ret);
}
