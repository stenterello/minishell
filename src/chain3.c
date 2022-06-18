/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:34:31 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/14 20:34:35 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_spaces(char *line, int i)
{
	while (line[i] && line[i] == ' ')
		i++;
	return (i);
}

int	n_lvls(char **l)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (l[i])
	{
		if (!ft_strncmp("(", l[i], 1))
			ret++;
		i++;
	}
	return (ret);
}

void	create_exits(int **exits, char **u_lines)
{
	int	i;

	*exits = malloc(sizeof(int) * n_lvls(u_lines) + 1);
	if (!*exits)
		die("Malloc error");
	i = 0;
	while (i < n_lvls(u_lines))
		*exits[i++] = 0;
}

void	increment_couple(int *f, int *s)
{
	(*f)++;
	(*s)++;
}

void	sup_count_units(char *line, int *i, int *ret)
{
	if ((!ft_strncmp(&line[*i], "&&", 2) || !ft_strncmp(&line[*i], "||", 2))
		&& !is_open(line, *i))
	{
		*i += 2;
		(*ret)++;
	}
	else if ((line[*i] == '(' || line[*i] == ')') && !is_open(line, *i))
		increment_couple(i, ret);
	else if (line[*i] == '\'' || line[*i] == '\"')
		take_bonus(&line[*i], i, line[*i], ret);
	else if (line[*i] != ' ')
	{
		(*ret)++;
		while (line[*i] && (ft_strncmp(&line[*i], "&&", 2)
				&& ft_strncmp(&line[*i], "||", 2)
				&& line[*i] != '(' && line[*i] != ')'))
			(*i)++;
	}
	*i += skip_spaces(&line[*i], 0);
}
