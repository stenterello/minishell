#include "minishell.h"

extern void	rl_replace_line(const char *str, int n);

void	flush(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;

	if (g_term.child && sig == SIGINT)
	{
		kill(g_term.child, SIGINT);
		ft_putstr_fd("^C\n", STDOUT_FILENO);
	}
	else if (sig == SIGINT)
	{
		if (g_term.delimiter)
		{
			ft_putstr_fd("^C", STDOUT_FILENO);
			g_term.delimiter = -1;
		}
		rl_replace_line("", 0);
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		if (!g_term.delimiter)
			rl_redisplay();
		else
		{
			free(g_term.input.line);
			g_term.delimiter = 0;
			main_loop();
		}
	}
	else if (sig == SIGQUIT)
	{
		
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
