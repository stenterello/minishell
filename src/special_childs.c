/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_childs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 12:43:37 by ddelladi          #+#    #+#             */
/*   Updated: 2022/07/01 17:46:32 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	treat_suspended_cat(t_command *tmp, t_terminfo *terminfo)
{
	int	status;

	kill(g_child, 15);
	wait(&status);
	waitpid(g_child, &status, 0);
	restore_fd(tmp);
	terminfo->suspended_cat++;
	return ;
}

void	treat_heredoc_child(int *status, t_command *tmp, t_terminfo *terminfo)
{
	close(STDIN_FILENO);
	if (!tmp->delimiter)
		close(tmp->output_fd);
	waitpid(g_child, status, 0);
	if (WIFEXITED(*status))
		terminfo->last_exit = *status / 256;
	else
		terminfo->last_exit = *status;
	g_child = 0;
	terminfo->is_suspended = 0;
	restore_fd(tmp);
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

void	suspended_cat(t_terminfo *terminfo)
{
	int	i;

	i = 0;
	while (i < terminfo->suspended_cat)
	{
		terminfo->input->line = readline("");
		if (!ft_strlen(terminfo->input->line))
			ft_putchar_fd('\n', STDOUT_FILENO);
		free(terminfo->input->line);
		terminfo->input->line = NULL;
		i++;
	}
}
