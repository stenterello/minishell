/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:46 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:56:53 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_born(t_command *tmp, int status)
{
	if (!g_term.delimiter)
	{
		waitpid(g_term.child, &status, 0);
		if (WIFEXITED(status))
			g_term.last_exit = status / 256;
		else
			g_term.last_exit = status;
		g_term.child = 0;
		restore_fd(tmp);
	}
	else
	{
		close(STDIN_FILENO);
		if (!g_term.delimiter)
			close(tmp->output_fd);
		waitpid(g_term.child, &status, 0);
		if (WIFEXITED(status))
			g_term.last_exit = status / 256;
		else
			g_term.last_exit = status;
		g_term.child = 0;
		dup2(tmp->saved_in, STDIN_FILENO);
		close(tmp->saved_in);
	}
}

int	sup_ex(t_command *tmp)
{
	while (tmp)
	{
		if (!builtin(tmp))
		{
			if (tmp->input_line)
			{
				ft_putstr_fd(tmp->input_line, tmp->output_fd);
				close(tmp->output_fd);
			}
			else if (ft_strchr(tmp->cmd, '/') == NULL)
			{
				if (find_script(tmp) == -1)
					cmd_not_found(tmp);
			}
			else
			{
				if (is_directory(tmp))
					return (0);
				born_child(tmp);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}
