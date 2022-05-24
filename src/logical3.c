/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:34:31 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/24 18:55:17 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unit_len(char *line)
{
	int	i;

	i = 0;
	i = sup_unit_len(line, i);
	return (i);
}

int	next_unit(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (line[i] && line[i] == ' ')
		i++;
	while (line[i])
	{
		if (ft_isalnum(line[i]) || line[i] == ' ')
		{
			while ((ft_strncmp(&line[i], "&&", 2)
					&& ft_strncmp(&line[i], "||", 2)
					&& line[i] != ')' && line[i] != '(') && !is_open(line, i))
				increment_couple(&i, &ret);
			return (ret);
		}
		else if ((!ft_strncmp(&line[i], "&&", 2)
				|| !ft_strncmp(&line[i], "||", 2) || line[i] == ')'
				|| line[i] == '(') && !is_open(line, i))
			return (2);
		i++;
	}
	return (ret);
}

int	sup_unit_len(char *line, int i)
{
	int	ret;

	ret = 0;
	i += skip_spaces(line, i);
	while (line[i])
	{
		if (ft_isalnum(line[i]) || line[i] == ' ')
		{
			while ((ft_strncmp("&&", &line[i], 2)
					&& ft_strncmp("||", &line[i], 2)) && line[i] != '('
				&& line[i] != ')' && !is_open(line, i))
				increment_couple(&i, &ret);
			while (line[i - 1] == ' ')
				decrement_couple(&i, &ret);
			return (ret);
		}
		else if ((line[i] == ')' || line[i] == '(')
			&& !is_open(line, i))
			return (1);
		else if ((!ft_strncmp("&&", &line[i], 2)
				|| !ft_strncmp("||", &line[i], 2)) && !is_open(line, i))
			return (2);
		i++;
	}
	return (ret);
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
