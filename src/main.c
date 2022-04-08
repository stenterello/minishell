#include "minishell.h"

void	add_signals(void)
{

}

int	main(void)
{
	t_input		input;

	g_term = NULL;
	g_term = malloc(sizeof(t_term));
	if (g_term == NULL)
		die("Malloc error");
	g_term->termi = malloc(sizeof(struct termios));
	if (!g_term->termi)
		die("Malloc error");
	take_environ();
	add_signals();
	while (1)
	{
		init_terminal(getenv("TERM"));
		init_input(&input);
		take_input(&input);
		if (ft_strlen(input.line) > 0)
		{
			add_history(input.line);
			if (input.to_expand)
				try_expand(&input);
			execute(&input);
		}
		free(input.line);
	}
	rl_clear_history();
	free_env(g_term->env);
	free(g_term->termi);
	free(g_term);
	return (0);
}

/*

// setta le variabili ambientali (export, unset, env)

// aggiungi i segnali Ctrl-D Ctrl-C Ctrl-\ 

// implementa i redirezionamenti

// implementa le pipe

// aggiungi gli eseguibili
*/
