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
	// int	par;

	// i = 0;
	// par = 0;
	// if (line[i] == '(')
	// {
	// 	par++;
	// 	i++;
	// 	while (line[i])
	// 	{
	// 		if (line[i] == ')')
	// 			par--;
	// 		else if (line[i] == '(')
	// 			par++;
	// 		if (!par)
	// 			return (i + 1);
	// 		i++;
	// 	}
	// }
	i = 0;
	i = sup_unit_len(line, i);
	return (i);
}

int	next_unit(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if ((!ft_strncmp(&line[i], "&&", 2)
			|| !ft_strncmp(&line[i], "||", 2))
			&& !is_open(line, i))
			break ;
		else if (line[i] == ')')
			break ;
		i++;
	}
	if (line[i])
		i += 2;
	else
		return (0);
	while (!ft_isalnum(line[i]))
		i++;
	return (i);
}
