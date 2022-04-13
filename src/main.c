#include "minishell.h"

void	init_sh_var(t_term *term)
{
	term->var = malloc(sizeof(t_sh_var));
	if (!term->var)
		die("Malloc error");
}

int	main(void)
{
	t_command	cmd;
	t_term		*term;

	term = NULL;
	term = malloc(sizeof(t_term));
	if (term == NULL)
		die("Malloc error");
	term->termi = malloc(sizeof(struct termios));
	if (!term->termi)
		die("Malloc error");
	take_environ(term);
	init_sh_var(term);
	while (1)
	{
		init_terminal(getenv("TERM"), term);
		init_input_and_cmd(&term->input, &cmd);
		add_signals(term);
		take_input(&term->input);
		if (ft_strlen(term->input.line) > 0)
		{
			add_history(term->input.line);
			while (term->input.to_expand)
			{
				try_expand(&term->input, term);
				check(term->input.line, &term->input);
			}
			split_command(term->input.line, &cmd);
			execute(&cmd, term);
		}
		free(term->input.line);
	}
	rl_clear_history();
	free_env(term->env);
	free_sh(term->var);
	free(term->termi);
	free(term);
	return (0);
}

/*

// aggiungi i segnali Ctrl-D Ctrl-C Ctrl-\ 

// implementa i redirezionamenti

// implementa le pipe

// implementa exit status

*/
