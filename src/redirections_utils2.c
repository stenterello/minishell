/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:11:01 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/04 00:57:17 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	define_pipe(t_command *cmd)
{
	int			piped[2];
	t_command	*tmp;

	if (pipe(piped) == -1)
		die(strerror(errno));
	cmd->output_fd = piped[1];
	tmp = cmd->next;
	tmp->input_fd = piped[0];
	cmd->saved_out = dup(STDOUT_FILENO);
	close(STDOUT_FILENO);
	dup2(cmd->output_fd, STDOUT_FILENO);
	close(cmd->output_fd);
}

void	define_pipe_to(t_command *cmd)
{
	cmd->saved_in = dup(STDIN_FILENO);
	close(STDIN_FILENO);
	dup2(cmd->input_fd, STDIN_FILENO);
	close(cmd->input_fd);
}

void	define_heredoc_pipe(t_command *cmd)
{
	int			piped[2];
	t_command	*tmp;

	if (pipe(piped) == -1)
		die(strerror(errno));
	cmd->output_fd = piped[1];
	tmp = cmd->next;
	tmp->input_fd = piped[0];
	tmp->saved_in = dup(STDOUT_FILENO);
	close(STDIN_FILENO);
	dup2(tmp->input_fd, STDIN_FILENO);
	close(tmp->input_fd);
}