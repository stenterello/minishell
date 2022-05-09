/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 14:11:38 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 15:47:09 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_text(char *dst, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	malloc_c(&dst, ft_strlen(src));
	while (src[i])
	{
		if (src[i] != '\"')
			dst[j++] = src[i++];
		else
			i++;
	}
	dst[j] = '\0';
}

int	dollar_pos(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '$')
		i++;
	return (i);
}

int	until_dollar(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '$')
		i++;
	return (++i);
}

int	until_end_var_name(char *line, char *var)
{
	return (until_dollar(line) + ft_strlen(var));
}

char	*define_var_name(char *line)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	while (line[i] != '$')
		i++;
	j = ++i;
	while (ft_isalnum(line[i]) || line[i] == '?')
		i++;
	malloc_c(&ret, i - j + 2);
	ft_strlcpy(ret, &line[j], i - j + 1);
	return (ret);
}
