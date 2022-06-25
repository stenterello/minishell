/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:27 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/24 20:24:56 by gimartin         ###   ########.fr       */
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

int	check_exit_cmd(t_command *cmd, int i)
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

void	exit_cmd(t_command *cmd, t_terminfo *terminfo)
{
	int	i;

	i = 0;
	while (cmd->args[i] != NULL)
		i++;
	if (!check_exit_cmd(cmd, i))
		return ;
	if (cmd->args[1] && ft_atoi(cmd->args[1]) < 0)
		i = 256 - ft_atoi(cmd->args[1]);
	else if (cmd->args[1])
		i = ft_atoi(cmd->args[1]);
	else
		i = 0;
	reset_term(terminfo);
	ft_putendl_fd("exit", STDOUT_FILENO);
	free_array_of_array(cmd->args);
	if (cmd->portions)
		free_array_of_array(cmd->portions);
	free(cmd->cmd);
	free_array_of_array(terminfo->glob_environ);
	free_dict(terminfo->env);
	free_dict(terminfo->var);
	free(terminfo->input->line);
	exit(i);
}
