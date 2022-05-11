/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 11:02:12 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/10 11:16:01 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
