/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:37 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/10 16:12:35 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern void	rl_replace_line(const char *str, int n);

void	sig_int(void)
{
	if (g_term.delimiter)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		g_term.last_exit = 130;
	}
	else
	{
		rl_replace_line("", 0);
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sup_flush(void)
{
	kill(g_term.child, SIGINT);
	if (!g_term.top)
	{
		ft_putstr_fd("^C\n", STDOUT_FILENO);
		g_term.signaled = 1;
		g_term.last_exit = 130;
	}
	else
		g_term.last_exit = 0;
}

void	flush(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (g_term.child && sig == SIGINT)
		sup_flush();
	else if (g_term.child && sig == SIGQUIT)
	{
		kill(g_term.child, SIGQUIT);
		ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
		g_term.last_exit = 131;
	}
	else if (sig == SIGINT)
		sig_int();
	else if (sig == SIGQUIT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	add_signals(void)
{
	int	sigs[2];

	g_term.acts.sa_sigaction = &flush;
	sigemptyset(&g_term.acts.sa_mask);
	sigs[0] = sigaction(SIGINT, &g_term.acts, NULL);
	sigs[1] = sigaction(SIGQUIT, &g_term.acts, NULL);
	if (sigs[0] || sigs[1])
		die("Signal error");
}
