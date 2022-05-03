/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:10:18 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:57:56 by ddelladi         ###   ########.fr       */
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

int	check_redirection(char **tmp, t_command *cmd)
{
	int	i;

	i = 0;
	if (!tmp[i + 1])
		return (-1);
	if (is_redir(tmp[i]) == 2)
		define_append_output(tmp[++i], cmd);
	else if (is_redir(tmp[i]) == 0)
		define_input(tmp[++i], cmd);
	else if (is_redir(tmp[i]) == 1)
		define_output(tmp[++i], cmd);
	return (0);
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
		else if (line[i] == '|' && !d_quot && !s_quot && line[i + 1] != '|' && line[i - 1] != '|')
			cmd->to_pipe = 1;
		i++;
	}
}
