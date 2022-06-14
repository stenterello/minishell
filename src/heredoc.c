/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:06:02 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/14 14:19:27 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	delimiter_len(char *line)
{
	int	i;

	i = 0;
	while (ft_isalnum(line[i]) || is_open(line, i))
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
	malloc_c(&ret, delimiter_len(&line[i]) + 1);
	ft_strlcpy(ret, &line[i], delimiter_len(&line[i]) + 1);
	return (ret);
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
