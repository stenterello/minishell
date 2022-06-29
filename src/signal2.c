/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 18:46:40 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/29 11:25:08 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	top_child_signal(int sig)
{
	if (sig == SIGINT)
	{
		kill(g_child, SIGINT);
		rl_redisplay();
	}
}

void	first_struct(struct sigaction act, t_command *cmd)
{
	int	sig;

	ft_memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	if (cmd && cmd->delimiter)
		act.sa_handler = &parent_signals_heredoc;
	else if (g_child && cmd->cmd
		&& (!ft_strncmp("top\0", &cmd->cmd[ft_strlen(cmd->cmd) - 3], 4)
			|| !ft_strncmp("ping\0", &cmd->cmd[ft_strlen(cmd->cmd) - 4], 5)))
		act.sa_handler = &top_child_signal;
	else if (g_child)
		act.sa_handler = &child_signals;
	else
		act.sa_handler = &parent_signals;
	sig = sigaction(SIGINT, &act, NULL);
	if (sig)
		die("Signal error");
}

void	second_struct_ign(struct sigaction act)
{
	int	sig;

	ft_memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_handler = SIG_IGN;
	sig = sigaction(SIGQUIT, &act, NULL);
	if (sig)
		die("Signal error");
}

void	second_struct_quit(struct sigaction act, t_command *cmd)
{
	int	sig;

	ft_memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	if (cmd->delimiter)
		act.sa_handler = &parent_signals_heredoc;
	else if (g_child && cmd->cmd && !ft_strncmp("top\0",
			&cmd->cmd[ft_strlen(cmd->cmd) - 3], 4))
		act.sa_handler = &top_child_signal;
	else if (g_child)
		act.sa_handler = &child_signals;
	else
		act.sa_handler = &parent_signals;
	sig = sigaction(SIGQUIT, &act, NULL);
	if (sig)
		die("Signal error");
}

void	add_signals(t_terminfo *terminfo, t_command *cmd)
{
	first_struct(terminfo->acts[0], cmd);
	if (!cmd)
		second_struct_ign(terminfo->acts[1]);
	else
		second_struct_quit(terminfo->acts[1], cmd);
}
