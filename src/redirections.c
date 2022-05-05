/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:10:22 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/04 00:57:28 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	define_input(char *line, t_command *cmd)
{
	if (cmd->input_fd != 0)
	{
		close(cmd->input_fd);
		dup2(cmd->saved_in, STDIN_FILENO);
		close(cmd->saved_in);
	}
	cmd->input_fd = open(line, O_CREAT, 0664);
	if (cmd->input_fd < 0)
		die(strerror(errno));
	cmd->saved_in = dup(STDIN_FILENO);
	close(STDIN_FILENO);
	dup2(cmd->input_fd, STDIN_FILENO);
	close(cmd->input_fd);
	cmd->redir_in = 1;
}

void	define_output(char *line, t_command *cmd)
{
	if (cmd->output_fd != 1)
	{
		close(cmd->output_fd);
		dup2(cmd->saved_out, STDOUT_FILENO);
		close(cmd->saved_out);
	}
	unlink(line);
	cmd->output_fd = open(line, O_WRONLY | O_CREAT, 0664);
	if (cmd->output_fd < 0)
		die(strerror(errno));
	cmd->saved_out = dup(1);
	close(1);
	dup2(cmd->output_fd, STDOUT_FILENO);
	close(cmd->output_fd);
	cmd->redir_out = 1;
}

void	define_append_output(char *line, t_command *cmd)
{
	if (cmd->output_fd != 1)
	{
		close(cmd->output_fd);
		dup2(cmd->saved_out, STDOUT_FILENO);
		close(cmd->saved_out);
	}
	cmd->output_fd = open(line, O_WRONLY | O_APPEND | O_CREAT, 0664);
	if (cmd->output_fd < 0)
		die(strerror(errno));
	cmd->saved_out = dup(STDOUT_FILENO);
	close(STDOUT_FILENO);
	dup2(cmd->output_fd, STDOUT_FILENO);
	close(cmd->output_fd);
	cmd->redir_out = 1;
}
