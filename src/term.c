/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:30 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/09 16:07:57 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_term(void)
{
	int	ret;

	ret = tcsetattr(STDIN_FILENO, 0, g_term.old_term);
	if (ret < 0)
		die(strerror(errno));
	free(g_term.old_term);
}

void	get_term(char *line)
{
	int	ret;

	ret = tgetent(NULL, line);
	if (ret == -1)
		die("Terminal database could not be found");
	else if (ret == 0)
		die("No such entry for TERM variable");
}

void	save_term(struct termios **terminal)
{
	int	ret;

	*terminal = malloc(sizeof(struct termios));
	if (!*terminal)
		die("Malloc error");
	ret = tcgetattr(STDIN_FILENO, *terminal);
	if (ret < 0)
		die(strerror(errno));
}

void	init_terminal(char *line)
{
	int	ret;

	get_term(line);
	save_term(&g_term.termi);
	ft_bzero(&g_term.termi->c_lflag, sizeof(tcflag_t));
	g_term.termi->c_lflag |= (ICANON | ISIG | IEXTEN | ECHO
			| ECHOE | ECHOK | ECHOKE | PENDIN);
	g_term.termi->c_lflag |= ~(ECHONL | ECHOPRT | NOFLSH
			| TOSTOP | FLUSHO | EXTPROC | ECHOCTL);
	ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, g_term.termi);
	if (ret < 0)
		die(strerror(errno));
	g_term.child = 0;
	g_term.delimiter = 0;
	g_term.suspended_cat = 0;
	g_term.is_suspended = 1;
	g_term.top = 0;
	g_term.terminated = 0;
}
