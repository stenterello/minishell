/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:46 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/06 15:00:34 by gimartin         ###   ########.fr       */
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
		if (WIFEXITED(status))
			g_term.last_exit = status / 256;
		else
			g_term.last_exit = status;
		g_term.child = 0;
		restore_fd(tmp);
		g_term.is_suspended = 0;
	}
	else
		treat_heredoc_child(&status, tmp);
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

int	infinite_exit(t_command *tmp)
{
	t_command	*next;

	next = tmp;
	while (next)
	{
		if (ft_strncmp(tmp->cmd, "exit\0", 5))
			return (0);
		next = next->next;
	}
	return (1);
}

int	preliminary(t_command *tmp)
{
	t_command	*bench;

	bench = tmp->next;
	if (!tmp->cmd && !g_term.delimiter)
	{
		treat_var_decl(tmp);
		return (1);
	}
	else if (!tmp->cmd && tmp->args && ft_strchr(tmp->args[0], '=')
		&& tmp->args[0][0] != '=' && tmp->args[1])
		rewrite_args(tmp);
	else if (infinite_exit(tmp) && bench && !ft_strncmp(bench->cmd, "exit\0", 5))
	{
		free_commands(tmp);
		return (1);
	}
	return (0);
}

void	free_commands(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i = 0;
		if (tmp->args)
		{
			while (tmp->args[i])
				free(tmp->args[i++]);
			free(tmp->args);
		}
		if (tmp->cmd)
			free(tmp->cmd);
		if (tmp->input_line)
			free(tmp->input_line);
		if (!tmp->first)
			free(tmp);
		tmp = tmp->next;
	}
	tmp = cmd;
}
