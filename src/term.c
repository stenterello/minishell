/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:30 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/29 11:20:19 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_term(t_terminfo *terminfo)
{
	int	ret;

	ret = tcsetattr(STDIN_FILENO, 0, terminfo->old_term);
	if (ret < 0)
		die(strerror(errno));
	free(terminfo->old_term);
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

void	init_terminal(char *line, t_terminfo *terminfo)
{
	int	ret;

	get_term(line);
	save_term(&terminfo->new_term);
	ft_bzero(&terminfo->new_term->c_lflag, sizeof(tcflag_t));
	terminfo->new_term->c_lflag |= (ICANON | ISIG | IEXTEN | ECHO
			| ECHOE | ECHOK | ECHOKE | PENDIN);
	terminfo->new_term->c_lflag |= ~(ECHONL | ECHOPRT | NOFLSH
			| TOSTOP | FLUSHO | EXTPROC | ECHOCTL);
	ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, terminfo->new_term);
	if (ret < 0)
		die(strerror(errno));
	terminfo->suspended_cat = 0;
	terminfo->is_suspended = 1;
	terminfo->top = 0;
	terminfo->signaled = 0;
}
