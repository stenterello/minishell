/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:07:01 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/15 10:14:06 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_chained(char *line)
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
