/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:10:22 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/30 16:34:55 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	define_input(char *line, t_command *cmd, t_terminfo *terminfo)
{
	if (cmd->input_fd != 0)
	{
		close(cmd->input_fd);
		dup2(cmd->saved_in, STDIN_FILENO);
		close(cmd->saved_in);
	}
	cmd->input_fd = open(line, O_RDONLY, 0664);
	if (cmd->input_fd < 0)
	{
		ft_putstr_fd(ft_getenv("SHELL", terminfo), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(line, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	cmd->saved_in = dup(STDIN_FILENO);
	close(STDIN_FILENO);
	dup2(cmd->input_fd, STDIN_FILENO);
	close(cmd->input_fd);
	cmd->redir_in = 1;
	return (0);
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

void	define_pipe_to(t_command *cmd)
{
	if (!cmd->redir_in)
	{
		cmd->saved_in = dup(STDIN_FILENO);
		close(STDIN_FILENO);
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
}

void	check_pipe(char *line, t_command *cmd)
{
	int			i;
	int			s_quot;
	int			d_quot;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		else if (line[i] == '"' && !d_quot && !s_quot)
			d_quot = 1;
		else if (line[i] == '"' && d_quot && !s_quot)
			d_quot = 0;
		else if (line[i] == '|' && !d_quot && !s_quot
			&& line[i + 1] != '|' && line[i - 1] != '|')
			cmd->to_pipe = 1;
		i++;
	}
}
