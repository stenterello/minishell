/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:37 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/09 18:34:18 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern void	rl_replace_line(const char *str, int n);

void	sig_int(void)
{
	if (g_term.delimiter)
	{
		ft_putstr_fd("\n", STDIN_FILENO);
		g_term.delimiter = 0;
		g_term.terminated = 1;
	}
	else
	{
		rl_replace_line("", 0);
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_redisplay();
		g_term.terminated = 1;
	}
}

void	sup_flush(void)
{
	kill(g_term.child, SIGINT);
	if (!g_term.top)
	{
		ft_putstr_fd("^C\n", STDOUT_FILENO);
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
	{
		sup_flush();
	}
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
