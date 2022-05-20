/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:07:01 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 20:50:31 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_logical(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_strlen(line) - i > 1)
	{
		if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2))
			&& !is_open(line, i))
			return (1);
		i++;
	}
	return (0);
}

int	ret_is_in_par(char *line, int limit, int s_quot, int d_quot)
{
	int	i;
	int	in_par;

	i = 0;
	in_par = 0;
	while (line[i] && i <= limit)
	{
		if (line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		else if (line[i] == '\"' && !d_quot && !s_quot)
			d_quot = 1;
		else if (line[i] == '\"' && d_quot && !s_quot)
			d_quot = 0;
		else if (line[i] == '(' && !d_quot && !s_quot)
			in_par++;
		else if (line[i] == ')' && !d_quot && !s_quot)
			in_par--;
		i++;
	}
	return (in_par);
}

int	is_in_par(char *line, int limit)
{
	int	in_par;
	int	s_quot;
	int	d_quot;

	s_quot = 0;
	d_quot = 0;
	in_par = ret_is_in_par(line, limit, s_quot, d_quot);
	if (in_par)
		return (1);
	return (0);
}

int	count_units(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 1;
	while (line[i])
	{
		if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2))
			&& !is_open(line, i))
		{
			i += 2;
			ret++;
		}
		else
			i++;
	}
	return (ret);
}

int	sup_unit_len(char *line, int i)
{
	while (line[i])
	{
		if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2))
			&& !is_open(line, i))
		{
			while (!ft_isalnum(line[i]) && line[i] != '\''
				&& line[i] != '\"' && line[i] != ')')
				i--;
			return (i + 1);
		}
		if (line[i] == ')')
			return (i);
		i++;
	}
	return (i);
}
