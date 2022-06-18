/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_childs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 12:43:37 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/15 16:54:28 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	treat_suspended_cat(t_command *tmp, t_terminfo *terminfo)
{
	kill(g_child, 15);
	restore_fd(tmp, terminfo);
	terminfo->suspended_cat++;
	return ;
}

void	treat_heredoc_child(int *status, t_command *tmp, t_terminfo *terminfo)
{
	close(STDIN_FILENO);
	if (!terminfo->delimiter)
		close(tmp->output_fd);
	waitpid(g_child, status, 0);
	if (WIFEXITED(*status))
		terminfo->last_exit = *status / 256;
	else
		terminfo->last_exit = *status;
	g_child = 0;
	dup2(tmp->saved_in, STDIN_FILENO);
	close(tmp->saved_in);
	terminfo->is_suspended = 0;
}

int	cmd_exists(char *line, t_terminfo *terminfo)
{
	if (access(line, X_OK))
	{
		ft_putstr_fd(ft_getenv("SHELL", terminfo), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(line, 2);
		terminfo->last_exit = 127;
		return (0);
	}
	return (1);
}
