/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:46 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/13 22:32:32 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_born(t_command *tmp, int status)
{
	if (g_term.is_suspended && !g_term.delimiter
		&& !ft_strncmp("cat\0", &tmp->cmd[ft_strlen(tmp->cmd) - 3], 4)
		&& tmp->next != NULL && !tmp->args[1])
		return (treat_suspended_cat(tmp));
	else if (!g_term.delimiter)
	{
		waitpid(g_term.child, &status, 0);
		if (!g_term.signaled)
		{
			if (WIFEXITED(status))
				g_term.last_exit = status / 256;
			else
				g_term.last_exit = status;
		}
		g_term.child = 0;
		restore_fd(tmp);
		g_term.is_suspended = 0;
	}
	else
		treat_heredoc_child(&status, tmp);
}

void	restore_all(t_command *cmd)
{
	t_command	*tmp;

	tmp = cmd;
	while (tmp)
	{
		restore_fd(tmp);
		tmp = tmp->next;
	}
	return ;
}

int	permitted(t_command *tmp)
{
	if (!access(tmp->cmd, F_OK) && access(tmp->cmd, X_OK) == -1)
	{
		g_term.last_exit = 126;
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(tmp->cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	sup_ex(t_command *tmp)
{
	t_command	*f;

	f = tmp;
	while (tmp)
	{
		if (!builtin(tmp))
		{
			if (tmp->input_line || (g_term.delimiter
					&& !tmp->input_line && !tmp->cmd))
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
				if (is_directory(tmp) || !permitted(tmp))
					return (0);
				born_child(tmp);
			}
		}
		tmp = tmp->next;
	}
	restore_all(f);
	return (1);
}

int	infinite_exit(t_command *tmp)
{
	t_command	*next;

	next = tmp;
	while (next)
	{
		if (tmp->cmd && ft_strncmp(tmp->cmd, "exit\0", 5))
			return (0);
		next = next->next;
	}
	return (1);
}
