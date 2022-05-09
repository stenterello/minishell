/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:36:08 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 13:38:16 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	key_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	if (!line[i] && search_vars(line, g_term.var) == NULL)
		return (-1);
	return (i);
}

int	value_len(char *line)
{
	int	i;
	int	j;

	i = key_len(line) + 1;
	if (line[i] == ' ')
		return (0);
	while (line[i] == '\"' || line[i] == '\'')
		i++;
	j = i;
	while (line[i] && line[i] != '"' && line[i] != '\'')
		i++;
	if (i - j == 0)
		return (-1);
	return (i - j);
}
