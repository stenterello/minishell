#include "minishell.h"

void	add_signals(void)
{

}

int	main(void)
{
	t_input		input;
	t_term		*term;

	term = NULL;
	term = malloc(sizeof(t_term));
	if (term == NULL)
		die("Malloc error");
	term->termi = malloc(sizeof(struct termios));
	if (!term->termi)
		die("Malloc error");
	take_environ(term);
	add_signals();
	while (1)
	{
		init_terminal(getenv("TERM"), term);
		init_input(&input);
		take_input(&input);
		if (ft_strlen(input.line) > 0)
		{
			add_history(input.line);
			if (input.to_expand)
				try_expand(&input, term);
			execute(&input, term);
		}
		free(input.line);
	}
	rl_clear_history();
	free_env(term->env);
	free(term->termi);
	free(term);
	return (0);
}

/*

// setta le variabili ambientali (export, unset, env)

// aggiungi i segnali Ctrl-D Ctrl-C Ctrl-\ 

// implementa i redirezionamenti

// implementa le pipe

// aggiungi gli eseguibili
*/
