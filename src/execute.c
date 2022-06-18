/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:42 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/17 18:11:10 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rewrite_args(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (is_var_def(cmd->args[i]))
		i++;
	malloc_c(&cmd->cmd, ft_strlen(cmd->args[i]) + 1);
	ft_strlcpy(cmd->cmd, cmd->args[i], ft_strlen(cmd->args[i]) + 1);
	while (cmd->args[i])
	{
		free(cmd->args[j]);
		malloc_c(&cmd->args[j], ft_strlen(cmd->args[i]) + 1);
		ft_strlcpy(cmd->args[j], cmd->args[i], ft_strlen(cmd->args[i]) + 1);
		free(cmd->args[i]);
		cmd->args[i] = NULL;
		i++;
		j++;
	}
	while (cmd->args[j])
	{
		free(cmd->args[j]);
		cmd->args[j++] = NULL;
	}
}

int	permitted(t_command *tmp, t_terminfo *terminfo)
{
	if (!access(tmp->cmd, F_OK) && access(tmp->cmd, X_OK) == -1)
	{
		terminfo->last_exit = 126;
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(tmp->cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	preliminary(t_command *tmp, t_terminfo *terminfo)
{
	t_command	*bench;

	bench = tmp->next;
	if (!tmp->cmd && !terminfo->delimiter)
	{
		treat_var_decl(tmp, terminfo);
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

void	write_and_close(t_command *tmp)
{
	ft_putstr_fd(tmp->input_line, tmp->output_fd);
	close(tmp->output_fd);
}

int	standard_execution(t_command *tmp, t_terminfo *terminfo)
{
	while (tmp)
	{
		if (!builtin(tmp, terminfo))
		{
			if (tmp->input_line || (terminfo->delimiter
					&& !tmp->input_line && !tmp->cmd))
				write_and_close(tmp);
			else if (ft_strchr(tmp->cmd, '/') == NULL)
			{
				if (find_script(tmp, terminfo) == -1)
					cmd_not_found(tmp, terminfo);
			}
			else
			{
				if (is_directory(tmp, terminfo)
					|| !permitted(tmp, terminfo))
					return (0);
				born_child(tmp, terminfo);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

void	execute_tree(t_command *cmd, t_terminfo *terminfo)
{
	t_command	*tmp;
	int			ret;

	tmp = cmd;
	if (preliminary(tmp, terminfo))
		return ;
	ret = standard_execution(tmp, terminfo);
	if (!ret)
		return ;
	if (!terminfo->delimiter)
		free_commands(cmd);
}
