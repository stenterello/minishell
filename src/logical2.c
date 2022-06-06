/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:07:01 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/24 18:55:26 by gimartin         ###   ########.fr       */
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

void	take_bonus(char *line, int *i, char c, int *ret)
{
	int	j;

	j = 1;
	while (line[j] != c)
	{
		j++;
		(*i)++;
	}
	(*ret)++;
}

int	count_units(char *line, int i)
{
	int	ret;

	ret = 0;
	while (line[i])
	{
		sup_count_units(line, &i, &ret);
	}
	return (ret);
}
