/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dict_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 10:59:10 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/15 12:32:18 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	key_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '=' && ft_strncmp(&line[i], "+=", 2))
		i++;
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

int	var_exists(t_dict *new, t_command *cmd, t_terminfo *terminfo)
{
	new->value = NULL;
	new->value = search_vars_value(cmd->args[1], terminfo->var);
	if (!new->value)
		return (0);
	return (1);
}

int	equal_char_index(t_command *cmd, int ind)
{
	int	j;

	j = 0;
	while (cmd->args[ind][j] && cmd->args[ind][j] != '='
		&& ft_strncmp(&cmd->args[ind][j], "+=", 2))
		j++;
	if (!ft_strncmp(&cmd->args[ind][j], "+=", 2))
		j++;
	return (j);
}
