/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 14:58:30 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/13 18:18:02 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_tree(t_command *cmd)
{
	t_command	*tmp;
	int			ret;

	tmp = cmd;
	if (preliminary(tmp))
		return ;
	ret = sup_ex(tmp);
	if (!ret)
		return ;
	if (!g_term.delimiter)
		free_commands(cmd);
}

void	sup_born_child(t_command *tmp, int status)
{
	g_term.child = fork();
	if (g_term.child == -1)
		die(strerror(errno));
	if (g_term.child == 0)
	{
		if (!ft_strncmp(&tmp->cmd[ft_strlen(tmp->cmd) - 9], "minishell", 9))
		{
			next_level();
			execve(tmp->cmd, tmp->args, g_term.glob_environ);
		}
		else
			execve(tmp->cmd, tmp->args, NULL);
	}
	else
		sup_born(tmp, status);
}

void	born_child(t_command *tmp)
{
	int			status;

	status = 0;
	if (!cmd_exists(tmp->cmd))
		return ;
	if (tmp->to_pipe)
		define_pipe(tmp);
	if (tmp->to_pipe_to)
		define_pipe_to(tmp);
	signal(SIGQUIT, &flush2);
	if (!ft_strncmp(&tmp->cmd[ft_strlen(tmp->cmd) - 3], "top", 3))
	{
		g_term.top = 1;
		g_term.child = fork();
		if (g_term.child == 0)
			execve(tmp->cmd, tmp->args, g_term.glob_environ);
		else
			wait(&status);
	}
	else
	{
		sup_born_child(tmp, status);
	}
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
