/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:58 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/15 15:51:00 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_builtin(t_command *cmd, t_terminfo *terminfo)
{
	char	*ret;

	ret = pwd(terminfo);
	if (cmd->to_pipe)
		define_pipe(cmd);
	if (cmd->to_pipe_to)
		define_pipe_to(cmd);
	ft_putendl_fd(ret, 1);
	free(ret);
}

int	builtin(t_command *cmd, t_terminfo *terminfo)
{
	if (!cmd->cmd)
		return (0);
	if (!ft_strncmp(cmd->cmd, "pwd\0", 4))
		sup_builtin(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "cd\0", 3))
		cd(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "echo\0", 5))
		echo(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "exit\0", 5))
		exit_cmd(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "env\0", 4))
		env(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "export\0", 7))
		export(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "unset\0", 6))
		unset(cmd, terminfo);
	else
		return (0);
	if (cmd->to_pipe || cmd->to_pipe_to || cmd->redir_in || cmd->redir_out)
		restore_fd(cmd, terminfo);
	return (1);
}
