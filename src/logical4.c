/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 12:53:03 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/14 14:21:58 by ddelladi         ###   ########.fr       */
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
