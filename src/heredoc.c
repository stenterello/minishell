/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:06:02 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/13 18:51:57 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	while (!ft_isalnum(line[i]) && line[i] != '\'' && line[i] != '"')
		i++;
	if (line[i] == '\'' || line[i] == '"')
		i++;
	malloc_c(&ret, delimiter_len(&line[i]) + 1);
	ft_strlcpy(ret, &line[i], delimiter_len(&line[i]) + 1);
	return (ret);
}

int	to_continue(char *typed, char *delimiter)
{
	if (ft_strlen(typed) == ft_strlen(delimiter)
		&& !ft_strncmp(typed, delimiter, ft_strlen(delimiter)))
		return (0);
	return (1);
}

void	clean_heredoc(char *line, char *bench)
{
	int	i;

	i = 0;
	while (ft_strncmp(&line[i], bench, ft_strlen(bench))
		&& i <= (int)(ft_strlen(line) - (int)ft_strlen(bench)))
		i++;
	while (!ft_isalnum(line[i]))
		i--;
	line[++i] = '\0';
}

char	*to_string(char *line)
{
	int		i;
	char	*ret;

	ret = NULL;
	malloc_c(&ret, ft_strlen(line) + 3);
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
