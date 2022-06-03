/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:36:08 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/15 16:19:01 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	key_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '=' && ft_strncmp(&line[i], "+=", 2))
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

int	check_export(t_command *cmd)
{
	if (cmd->args[1][0] == '=')
	{
		ft_putstr_fd(ft_getenv("SHELL"), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(cmd->args[1] + 1, 2);
		ft_putendl_fd(" not found", 2);
		g_term.last_exit = 127;
		return (1);
	}
	return (0);
}
