/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:21 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/14 14:22:22 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sup_echo(t_command *cmd)
{
	int		i;

	i = 1;
	while (!ft_strncmp("-n\0", cmd->args[i], 3))
		i++;
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i++], STDOUT_FILENO);
		if (cmd->args[i])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (ft_strncmp("-n\0", cmd->args[1], 3))
		ft_putchar_fd('\n', STDOUT_FILENO);
}

void	echo(t_command *cmd)
{
	if (cmd->to_pipe)
		define_pipe(cmd);
	if (cmd->to_pipe_to)
		define_pipe_to(cmd);
	if (!cmd->args[1])
		ft_putendl_fd(NULL, 1);
	else
		sup_echo(cmd);
	g_term.last_exit = 0;
}
