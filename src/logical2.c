/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:07:01 by gimartin          #+#    #+#             */
/*   Updated: 2022/04/28 14:29:50 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_logical(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_strlen(line) - i > 1)
	{
		if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2)) && !is_open(line, i))
			return (1);
		i++;
	}
	return (0);
}

int	is_in_par(char *line, int limit)
{
	int	i;
	int	in_par;
	int	s_quot;
	int	d_quot;

	i = 0;
	in_par = 0;
	s_quot = 0;
	d_quot = 0;
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
		if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2)) && !is_open(line, i) && !is_in_par(line, i))
		{
			i += 2;
			ret++;
		}
		else
			i++;
	}
	return (ret);
}

int	unit_len(char *line)
{
	int	i;
	int	par;

	i = 0;
	par = 0;
	if (line[i] == '(')
	{
		par++;
		i++;
		while (line[i])
		{
			if (line[i] == ')')
				par--;
			else if (line[i] == '(')
				par++;
			if (!par)
				return (i + 1);
			i++;
		}
	}
	while (line[i])
	{
		if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2)) && !is_open(line, i) && !is_in_par(line, i))
		{
			while (!ft_isalnum(line[i]) && line[i] != '\'' && line[i] != '\"' && line[i] != ')')
				i--;
			return (i + 1);
		}
		i++;
	}
	return (i);
}

int	next_unit(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2)) && !is_open(line, i) && !is_in_par(line, i))
			break ;
		i++;
	}
	if (line[i])
		i += 2;
	else
		return (0);
	while (!ft_isalnum(line[i]) && line[i] != '(')
		i++;
	return (i);
}
