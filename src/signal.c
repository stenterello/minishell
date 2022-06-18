/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:37 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/18 18:28:45 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern void	rl_replace_line(const char *str, int n);

// void	sig_int(void)
// {
// 	// if (g_term.delimiter)
// 	// {
// 	// 	ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 	// 	rl_replace_line("", 0);
// 	// 	rl_on_new_line();
// 	// 	g_term.last_exit = 130;
// 	// }
// 	// else
// 	// {
// 		rl_replace_line("", 0);
// 		ft_putchar_fd('\n', 1);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	// }
// }


void	heredoc_sigint(void)
{
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	sigint(void)
{
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_replace_line("", STDIN_FILENO);
	rl_on_new_line();
	rl_redisplay();
}

void	parent_signals(int sig)
{
	if (sig == SIGINT)
		sigint();
}

void	parent_signals_heredoc(int sig)
{
	if (sig == SIGINT)
		heredoc_sigint();
}

void	child_signals(int sig)
{
	if (sig == SIGINT)
	{
		kill(g_child, SIGINT);
		ft_putendl_fd("^C", STDOUT_FILENO);
	}
	else if (sig == SIGQUIT)
	{
		kill(g_child, SIGINT);
		ft_putendl_fd("^\\Quit: 3", STDOUT_FILENO);
	}
}

void	top_child_signal(int sig)
{
	if (sig == SIGINT)
	{
		kill(g_child, SIGINT);
		rl_redisplay();
	}
}

void	first_struct(struct sigaction act, t_command *cmd, t_terminfo *terminfo)
{
	int	sig;

	ft_memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	if (terminfo->delimiter)
		act.sa_handler = &parent_signals_heredoc;
	else if (g_child && cmd->cmd && (!ft_strncmp("top\0", &cmd->cmd[ft_strlen(cmd->cmd) - 3], 4) || !ft_strncmp("ping\0", &cmd->cmd[ft_strlen(cmd->cmd) - 4], 5)))
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

void	second_struct_quit(struct sigaction act, t_command *cmd, t_terminfo *terminfo)
{
	int	sig;

	ft_memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	if (terminfo->delimiter)
		act.sa_handler = &parent_signals_heredoc;
	else if (g_child && cmd->cmd && !ft_strncmp("top\0", &cmd->cmd[ft_strlen(cmd->cmd) - 3], 4))
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
	first_struct(terminfo->acts[0], cmd, terminfo);
	if (!cmd)
		second_struct_ign(terminfo->acts[1]);
	else
		second_struct_quit(terminfo->acts[1], cmd, terminfo);
}
