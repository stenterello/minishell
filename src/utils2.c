/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 14:11:38 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/14 14:16:32 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	until_dollar(char *line)
{
	int		i;
	int		s_quot;
	int		d_quot;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		if (line[i] == '"' && !s_quot && !d_quot)
			d_quot = 1;
		else if (line[i] == '"' && !s_quot && d_quot)
			d_quot = 0;
		if (line[i] == '$' && !s_quot)
		{
			i++;
			return (i);
		}
		i++;
	}
	return (0);
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
