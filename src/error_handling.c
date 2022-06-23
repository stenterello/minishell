/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:05 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/23 10:54:07 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_not_found(t_command *cmd, t_terminfo *terminfo)
{
	ft_putstr_fd(ft_getenv("SHELL", terminfo), STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(cmd->cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	terminfo->last_exit = 127;
}

char	*last_field(char *line)
{
	int	i;

	i = ft_strlen(line) - 1;
	while (line[i] && line[i] != '/')
		i--;
	return (&line[++i]);
}

int	is_directory(t_command *cmd, t_terminfo *terminfo)
{
	struct stat	file_stat;
	int			i;

	i = 0;
	if (!access(cmd->cmd, F_OK))
	{
		stat(cmd->cmd, &file_stat);
		if ((file_stat.st_mode & S_IFMT) == S_IFDIR)
		{
			ft_putstr_fd(last_field
				(ft_getenv("SHELL", terminfo)), STDOUT_FILENO);
			ft_putstr_fd(": ", STDOUT_FILENO);
			ft_putstr_fd(cmd->cmd, STDOUT_FILENO);
			ft_putendl_fd(": Is a directory", STDOUT_FILENO);
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
			free(cmd->cmd);
			terminfo->last_exit = 126;
			return (1);
		}
	}
	return (0);
}

int	syntax_error_no_arr(t_terminfo *terminfo)
{
	ft_putstr_fd(last_field(ft_getenv("SHELL", terminfo)), 2);
	ft_putendl_fd(": syntax error near unexpected token \"newline\"", 2);
	return (1);
}
