/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:11:01 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/13 16:06:37 by gimartin         ###   ########.fr       */
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

int	sup_check_red(char **tmp, int i, t_command *cmd)
{
	while (tmp[i] && is_redir(tmp[i]) == 0)
	{
		if (define_input(tmp[++i], cmd) != -1)
			i++;
		else
			return (-1);
	}
	return (0);
}
