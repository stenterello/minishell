/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 18:59:14 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/23 11:12:39 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	born_child(t_command *tmp, t_terminfo *terminfo)
{
	int			status;

	status = 0;
	if (!cmd_exists(tmp->cmd, terminfo))
		return ;
	if (tmp->redir_in)
		define_input_redirection(tmp->redi, tmp, terminfo);
	if (tmp->redir_out)
		define_output_redirection(tmp->redi, tmp);
	if (tmp->to_pipe)
		define_pipe(tmp);
	if (tmp->to_pipe_to)
		define_pipe_to(tmp);
	if (!ft_strncmp(&tmp->cmd[ft_strlen(tmp->cmd) - 3], "top", 3))
		terminfo->top = 1;
	maieutica(tmp, status, terminfo);
	if (!ft_strncmp(&tmp->cmd[ft_strlen(tmp->cmd) - 3], "top", 3))
		g_child = 0;
	if (g_child == -1)
	{
		g_child = 0;
		terminfo->last_exit = 131;
	}
}
