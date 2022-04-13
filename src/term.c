#include "minishell.h"

void	flush(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		// if (ft_strlen(g_term->input.line) > 0)
		// {
		// 	ft_putchar_fd('\n', 1);
		// 	ft_putendl_fd("exit", 1);
		// 	exit(0);
		// }
	}
}

void	add_signals(t_term *term)
{
	int	sigs[3];

	term->acts.sa_sigaction = &flush;
	sigemptyset(&term->acts.sa_mask);
	sigs[0] = sigaction(SIGINT, &term->acts, NULL);
	sigs[2] = sigaction(SIGQUIT, &term->acts, NULL);

	if (sigs[0] || sigs[2])
		die("Signal error");
}

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
	add_signals(term);
}
