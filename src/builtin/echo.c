/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:21 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/13 17:18:44 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quoted(char	*line)
{
	int	s_quot;
	int	d_quot;
	int	i;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !s_quot && !d_quot)
			return (1);
		else if (line[i] == '\"' && !d_quot && !s_quot)
			return (2);
		i++;
	}
	return (0);
}

void	sup_echo(t_command *cmd)
{
	int		i;

	i = 1;
	while (!ft_strncmp("-n", cmd->args[i], 2))
		i++;
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i++], STDOUT_FILENO);
		if (cmd->args[i])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (ft_strncmp("-n", cmd->args[1], 2))
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
