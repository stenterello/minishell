/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:46 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/29 11:22:40 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	free_commands(cmd);
}
