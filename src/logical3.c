/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:34:31 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 15:34:44 by gimartin         ###   ########.fr       */
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
			while ((ft_strncmp(&line[i], "&&", 2) && ft_strncmp(&line[i], "||", 2) && line[i] != ')' && line[i] != '(')
				&& !is_open(line, i))
			{
				i++;
				ret++;
			}
			return (ret);
		}
		else if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2) || line[i] == ')' || line[i] == '(')
			&& !is_open(line, i))
			return (2) ;
		i++;
	}
	return (ret);
}

// int	next_unit(char *line)
// {
// 	int	i;

// 	i = 0;
// 	while (line[i])
// 	{
// 		if ((!ft_strncmp(&line[i], "&&", 2)
// 			|| !ft_strncmp(&line[i], "||", 2))
// 			&& !is_open(line, i))
// 			break ;
// 		else if ((line[i] == '(' || line[i] == ')') && !is_open(line, i))
// 			return (1);
// 		i++;
// 	}
// 	if (line[i])
// 		i += 2;
// 	else
// 		return (0);
// 	while (!ft_isalnum(line[i]))
// 		i++;
// 	return (i);
// }
