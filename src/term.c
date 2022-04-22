#include "minishell.h"

void	reset_term(void)
{
	int	ret;

	ret = tcsetattr(STDIN_FILENO, 0, g_term.old_term);
	if (ret < 0)
		die("Error while resetting terminal configuration");
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
		die("Error while retrieving terminal configuration");
}

void	init_terminal(char *line)
{
	int	ret;

	get_term(line);
	save_term(&g_term.termi);
	ft_bzero(&g_term.termi->c_lflag, sizeof(tcflag_t));
	g_term.termi->c_lflag |= (ICANON | ISIG | IEXTEN | ECHO | ECHOE | ECHOK | ECHOKE | PENDIN);
	g_term.termi->c_lflag |= ~(ECHONL | ECHOPRT | /*ALTWERASE |*/ NOFLSH | TOSTOP | FLUSHO |/* NOKERNINFO |*/ EXTPROC | ECHOCTL);
	ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, g_term.termi);
	if (ret < 0)
		die("Error while initializing terminal attributes");
	g_term.child = 0;
	g_term.delimiter = 0;
}
