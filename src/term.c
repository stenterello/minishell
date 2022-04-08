#include "minishell.h"

void	term_data(char *line)
{
	int	ret;

	ret = tgetent(NULL, line);
	if (ret < 0)
		die("No such entry for TERM variable");
	else if (ret == -1)
		die("Terminal database could not be found");
}

void	init_terminal(char *line, t_term *term)
{
	int	ret;

	term_data(line);
	ret = tcgetattr(STDIN_FILENO, term->termi);
	if (ret < 0)
		die("tcgetattr error");
	term->termi->c_lflag &= (ECHO | ICANON);
	term->termi->c_cc[VMIN] = 1;
	term->termi->c_cc[VTIME] = 0;
	ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, term->termi);
	if (ret < 0)
		die("tcsetattr error");
}
