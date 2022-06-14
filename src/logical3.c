/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:34:31 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/14 14:21:35 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
