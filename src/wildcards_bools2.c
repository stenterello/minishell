/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_bools2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 11:24:19 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/13 16:26:35 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sup_brackets_is_verified_2(char **port, char *file, int i, int j)
{
	while (port[i] && port[i][0] == '[' && file[j])
	{
		while (file[j] && port[i]
			&& is_verified_brackets(port[i], file[j]) == 0)
			j++;
		if (is_verified_brackets(port[i], file[j]) == -1)
			return (-1);
		if (is_verified_brackets(port[i], file[j]) == 1)
		{
			i++;
			j++;
		}
		else if (!file[j])
			return (0);
	}
	return (1);
}

int	sup_string_is_verified(char **port, char *file, int i, int j)
{
	if (port[i] && i > 0 && port[i - 1][0] == '*'
			&& !is_wildcard(port[i][0]))
	{
		while (file[j] && ft_strncmp(&file[j], port[i], ft_strlen(port[i])))
			j++;
		if (!file[j])
			return (0);
		i++;
	}
	else
	{
		if (port[i] && !is_wildcard(port[i][0])
			&& !ft_strncmp(&file[j], port[i], ft_strlen(port[i])))
		{
			j += ft_strlen(port[i]);
			i++;
		}
		else if (port[i] && !is_wildcard(port[i][0]))
			return (0);
	}
	return (1);
}

int	sup_brackets_is_verified(char **port, int i, int j, char *file)
{
	int	ret;

	ret = 0;
	if (port[i] && i > 0 && port[i - 1][0] == '*'
		&& port[i][0] == '[' && file[j])
	{	
		ret = sup_brackets_is_verified_2(port, file, i, j);
		if (ret == 0 || ret == -1)
			return (ret);
	}
	else if (port[i] && port[i][0] == '[' && file[j])
	{
		if (is_verified_brackets(port[i], file[j]) == -1)
			return (-1);
		if (is_verified_brackets(port[i], file[j]) == 1)
		{
			i++;
			j++;
		}
		else
			return (0);
	}
	return (5);
}

int	sup_is_verified(char **port, int *c, char *file)
{
	while (port[c[0]] && port[c[0]][0] == '?')
	{
		c[0]++;
		c[1]++;
		if (!port[c[0]] && !file[c[1]])
			return (1);
		if (!file[c[1]])
			return (0);
		if (!port[c[0]])
			return (0);
	}
	return (2);
}

int	is_verified(char *file, char **port)
{
	int	c[4];

	c[0] = 0;
	c[1] = 0;
	c[3] = 0;
	while (port[c[0]] && file[c[1]])
	{
		while (port[c[0]] && port[c[0]][0] == '*')
			c[0]++;
		if (!port[c[0]])
			return (1);
		c[3] = sup_is_verified(port, c, file);
		if (c[3] != 2)
			return (c[3]);
		c[2] = sup_brackets_is_verified(port, c[0], c[1], file);
		if (c[2] != 5)
			return (c[2]);
		c[2] = sup_string_is_verified(port, file, c[0], c[1]);
		if (c[2] == 0)
			return (c[2]);
		else
			c[1] += ft_strlen(port[c[0]++]);
	}
	return (1);
}
