/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_childs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 12:43:37 by ddelladi          #+#    #+#             */
/*   Updated: 2022/05/24 13:15:11 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	treat_suspended_cat(t_command *tmp)
{
	kill(g_term.child, 15);
	restore_fd(tmp);
	g_term.suspended_cat++;
	return ;
}

void	treat_heredoc_child(int *status, t_command *tmp)
{
	close(STDIN_FILENO);
	if (!g_term.delimiter)
		close(tmp->output_fd);
	waitpid(g_term.child, status, 0);
	if (WIFEXITED(*status))
		g_term.last_exit = *status / 256;
	else
		g_term.last_exit = *status;
	g_term.child = 0;
	dup2(tmp->saved_in, STDIN_FILENO);
	close(tmp->saved_in);
	g_term.is_suspended = 0;
}

int	cmd_exists(char *line)
{
	if (access(line, X_OK))
	{
		ft_putstr_fd(ft_getenv("SHELL"), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(line, 2);
		g_term.last_exit = 127;
		return (0);
	}
	return (1);
}
