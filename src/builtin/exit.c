/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:27 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/13 17:01:25 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_number(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isdigit(line[i]))
			return (0);
		i++;
	}
	return (1);
}

int	sup_exit_cmd(t_command *cmd, int i)
{
	if (i > 2)
	{
		ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
		return (0);
	}
	else if (i == 2 && !is_number(cmd->args[1]))
	{
		ft_putendl_fd("exit: numeric argument required", STDERR_FILENO);
		return (0);
	}
	else if (cmd->to_pipe)
		return (0);
	return (1);
}

void	exit_cmd(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i] != NULL)
		i++;
	if (!sup_exit_cmd(cmd, i))
		return ;
	reset_term();
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (cmd->portions)
		free_array_of_array(cmd->portions);
	free(cmd->cmd);
	free_array_of_array(g_term.glob_environ);
	free_dict(g_term.env);
	free_dict(g_term.var);
	free(g_term.input.line);
	if (i == 1)
		exit(0);
	else if (ft_atoi(cmd->args[i - 1]) < 0)
		exit(256 - ft_atoi(cmd->args[i - 1]));
	else
		exit(ft_atoi(cmd->args[i - 1]));
}
