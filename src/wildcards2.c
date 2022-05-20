/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 16:12:15 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/13 16:13:02 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_wildcards(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (has_wildcard(cmd->args[i]))
		{
			if (guess(cmd, i) == -1)
				return (-1);
		}
		i++;
	}
	if (ft_strncmp(cmd->args[0], cmd->cmd, ft_strlen(cmd->args[0]) + 1))
	{
		free(cmd->cmd);
		malloc_c(&cmd->cmd, ft_strlen(cmd->args[0]) + 1);
		ft_strlcpy(cmd->cmd, cmd->args[0], ft_strlen(cmd->args[0]) + 1);
	}
	return (0);
}
