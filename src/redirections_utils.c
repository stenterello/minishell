/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:10:18 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/30 16:35:08 by ddelladi         ###   ########.fr       */
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
	if (cmd->redir_in || cmd->to_pipe_to || cmd->delimiter)
		restore_input(cmd);
}

void	sup_def_pipe(t_command *cmd, int *piped, t_command *tmp)
{
	tmp = cmd->next;
	if (!tmp->redir_in)
		tmp->input_fd = piped[0];
	else
	{
		tmp->to_pipe_to = 0;
		close(piped[0]);
	}
}

void	define_pipe(t_command *cmd)
{
	int			piped[2];
	t_command	*tmp;

	tmp = NULL;
	if (pipe(piped) == -1)
		die(strerror(errno));
	if (!cmd->redir_out)
	{
		cmd->output_fd = piped[1];
		cmd->saved_out = dup(STDOUT_FILENO);
		close(STDOUT_FILENO);
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
	else
	{
		cmd->to_pipe = 0;
		close(piped[1]);
	}
	sup_def_pipe(cmd, piped, tmp);
}
