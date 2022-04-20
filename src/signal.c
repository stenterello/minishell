#include "minishell.h"

extern void	rl_replace_line(const char *str, int n);

void	flush(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;

	if (g_term.child && sig == SIGINT)
	{
		kill(g_term.child, SIGINT);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	else if (sig == SIGINT)
	{
		// g_term.termi->c_lflag &= ~ (ECHO | ICANON);
    	// tcsetattr(STDIN_FILENO, TCSAFLUSH, g_term.termi);
		rl_replace_line("", 0);
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_redisplay();
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
	sigs[0] = sigaction(SIGINT, &g_term.acts, NULL);
	sigs[1] = sigaction(SIGQUIT, &g_term.acts, NULL);

	if (sigs[0] || sigs[1])
		die("Signal error");
}