/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:05 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/10 12:15:52 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(t_command *cmd)
{
	ft_putstr_fd(cmd->cmd, STDERR_FILENO);
	strerror(errno);
	g_term.last_exit = errno;
}

void	cmd_not_found(t_command *cmd)
{
	ft_putstr_fd(ft_getenv("SHELL"), STDERR_FILENO);
	ft_putstr_fd(": command not found: ", STDERR_FILENO);
	ft_putendl_fd(cmd->cmd, STDERR_FILENO);
	g_term.last_exit = 127;
}

char	*last_field(char *line)
{
	int	i;

	i = ft_strlen(line) - 1;
	while (line[i] && line[i] != '/')
		i--;
	return (&line[++i]);
}

int	is_directory(t_command *cmd)
{
	struct stat	file_stat;
	int			i;

	i = 0;
	if (!access(cmd->cmd, F_OK))
	{
		stat(cmd->cmd, &file_stat);
		if ((file_stat.st_mode & S_IFMT) == S_IFDIR)
		{
			ft_putstr_fd(last_field(ft_getenv("SHELL")), STDOUT_FILENO);
			ft_putstr_fd(": ", STDOUT_FILENO);
			ft_putstr_fd(cmd->cmd, STDOUT_FILENO);
			ft_putendl_fd(": Is a directory", STDOUT_FILENO);
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
			free(cmd->cmd);
			g_term.last_exit = 126;
			return (1);
		}
	}
	return (0);
}
