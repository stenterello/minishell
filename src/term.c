#include "minishell.h"

// void	kill_process(int sig)
// {

// }



void	term_data(char *line)
{
	int	ret;

	ret = tgetent(NULL, line);
	if (ret == -1)
		die("Terminal database could not be found");
	else if (ret < 0)
		die("No such entry for TERM variable");
}

void	init_terminal(char *line)
{
	int	ret;

	term_data(line);
	g_term.termi = malloc(sizeof(struct termios));
	if (!g_term.termi)
		die("Malloc error");
	ret = tcgetattr(STDIN_FILENO, g_term.termi);
	if (ret < 0)
		die("tcgetattr error");
	g_term.termi->c_lflag &= ~(ECHOCTL);
	// g_term.termi->c_cc[VMIN] = 1;
	// g_term.termi->c_cc[VTIME] = 0;
	ret = tcsetattr(STDIN_FILENO, TCSANOW, g_term.termi);
	if (ret < 0)
		die("tcsetattr error");
	g_term.child = 0;
}
