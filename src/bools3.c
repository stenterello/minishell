/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:58 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:56:21 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_builtin(t_command *cmd)
{
	char	*ret;

	ret = pwd();
	if (cmd->to_pipe)
		define_pipe(cmd);
	if (cmd->to_pipe_to)
		define_pipe_to(cmd);
	ft_putendl_fd(ret, 1);
	free(ret);
}

int	builtin(t_command *cmd)
{
	if (!cmd->cmd)
		return (0);
	if (!ft_strncmp(cmd->cmd, "pwd\0", 4))
		sup_builtin(cmd);
	else if (!ft_strncmp(cmd->cmd, "cd\0", 3))
		cd(cmd);
	else if (!ft_strncmp(cmd->cmd, "echo\0", 5))
		echo(cmd);
	else if (!ft_strncmp(cmd->cmd, "exit\0", 5))
		exit_cmd(cmd);
	else if (!ft_strncmp(cmd->cmd, "env\0", 4))
		env(cmd);
	else if (!ft_strncmp(cmd->cmd, "export\0", 7))
		export(cmd);
	else if (!ft_strncmp(cmd->cmd, "unset\0", 6))
		unset(cmd);
	else
		return (0);
	if (cmd->to_pipe || cmd->to_pipe_to || cmd->redir_in || cmd->redir_out)
		restore_fd(cmd);
	g_term.child = 0;
	return (1);
}

int	ft_isupper(char c)
{
	if (c >= 65 && c <= 90)
		return (1);
	return (0);
}
