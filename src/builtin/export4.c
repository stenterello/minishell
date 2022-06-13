/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 22:11:17 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/13 22:15:53 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_dict	*norm_sup_export(int j, t_command *cmd, t_dict *new)
{
	j++;
	if (cmd->args[1][j] == '\''
		&& cmd->args[1][ft_strlen(cmd->args[1]) - 1] == '\'')
	{
		malloc_c(&new->value, ft_strlen(&cmd->args[1][++j]));
		ft_strlcpy(new->value, &cmd->args[1][j],
			ft_strlen(&cmd->args[1][j]));
	}
	else
	{
		malloc_c(&new->value, ft_strlen(&cmd->args[1][j]));
		ft_strlcpy(new->value, &cmd->args[1][j],
			ft_strlen(&cmd->args[1][j]) + 1);
	}
	return (new);
}
