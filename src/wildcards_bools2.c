/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_bools2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 11:24:19 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/11 14:34:40 by ddelladi         ###   ########.fr       */
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
	if (port[i] && i > 0 && port[i - 1][0] == '*' && port[i][0] == '[' && file[j])
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

int	is_verified(char *file, char **port)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	j = 0;
	while (port[i] && file[j])
	{
		while (port[i] && port[i][0] == '*')
			i++;
		if (!port[i])
			return (1);
		while (port[i] && port[i][0] == '?')
		{
			i++;
			j++;
			if (!port[i] && !file[j])
				return (1);
			if (!file[j])
				return (0);
			if (!port[i])
				return (0);
		}
		ret = sup_brackets_is_verified(port, i, j, file);
		if (ret != 5)
			return (ret);
		ret = sup_string_is_verified(port, file, i, j);
		if (ret == 0)
			return (ret);
		else
			j += ft_strlen(port[i++]);
	}
	return (1);
}
