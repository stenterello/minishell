#include "minishell.h"

// void	kill_process(int sig)
// {

// }

void	flush(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	printf("%d\n", g_term.child);
	if (g_term.child != -1)
	{
		kill(g_term.child, 9);
		g_term.child = -1;
	}
	else if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_redisplay();
		g_term.last_exit = 130;
	}
	else if (sig == SIGQUIT)
	{
		ft_putchar_fd('\n', STDIN_FILENO);
		if (ft_strlen(g_term.input.line) > 1)
		{
			ft_putchar_fd('\n', 1);
			ft_putendl_fd("exit", 1);
			exit(0);
		}
		// else
		// {
		// 	g_term.input.line[ft_strlen(g_term.input.line) - 1] = '\0';
		// 	rl_replace_line(g_term.input.line, 0);
		// }
	}
}

void	add_signals(void)
{
	int	sigs[2];


	g_term.acts.sa_sigaction = &flush;
	sigemptyset(&g_term.acts.sa_mask);
	g_term.acts.sa_flags = SA_RESTART;
	sigs[0] = sigaction(SIGINT, &g_term.acts, NULL);
	sigs[1] = sigaction(SIGQUIT, &g_term.acts, NULL);

	if (sigs[0] || sigs[1])
		die("Signal error");
}

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
	g_term.termi->c_lflag &= (ECHO | ICANON);
	g_term.termi->c_cc[VMIN] = 1;
	g_term.termi->c_cc[VTIME] = 0;
	ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, g_term.termi);
	if (ret < 0)
		die("tcsetattr error");
	g_term.child = -1;
}
