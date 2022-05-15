/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:10:18 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/13 16:08:52 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_output(t_command *cmd)
{
	if (cmd->saved_out != 0)
	{
		close(STDOUT_FILENO);
		dup2(cmd->saved_out, STDOUT_FILENO);
		close(cmd->saved_out);
	}
}

void	restore_input(t_command *cmd)
{
	close(STDIN_FILENO);
	dup2(cmd->saved_in, STDIN_FILENO);
	close(cmd->saved_in);
}

void	restore_fd(t_command *cmd)
{
	if (cmd->redir_out || cmd->to_pipe)
		restore_output(cmd);
	if (cmd->redir_in || cmd->to_pipe_to)
		restore_input(cmd);
}

void	control_define1(char **tmp, t_command *cmd, int i)
{
	while (tmp[i] && is_redir(tmp[i]) == 2)
	{
		define_append_output(tmp[++i], cmd);
		i++;
	}
}

int	check_redirection(char **tmp, t_command *cmd)
{
	int	i;

	i = 0;
	if (!tmp[i + 1])
		return (-1);
	if (is_redir(tmp[i]) == 2)
		control_define1(tmp, cmd, i);
	else if (is_redir(tmp[i]) == 0)
	{
		if (sup_check_red(tmp, i, cmd) == -1)
			return (-1);
	}
	else if (is_redir(tmp[i]) == 1)
	{
		while (tmp[i] && is_redir(tmp[i]) == 1)
		{
			define_output(tmp[++i], cmd);
			i++;
		}
	}
	return (0);
}
