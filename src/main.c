#include "minishell.h"

// void	print_newline(int num)
// {

// }

// void	add_signals(void)
// {
// 	struct sigaction	d;
// 	struct sigaction	c;
// 	struct sigaction	back;
// 	int					*sigs;

// 	d.sa_sigaction = &print_newline;
// 	sigemptyset(&d.sa_mask);
// 	sigs[0] = sigaction(SIGINT, &d, NULL);

// 	if (sigs[0] || sigs[1] || sigs[2])
// 		die("Signal error");
// }

void	init_sh_var(t_term *term)
{
	term->var = malloc(sizeof(t_sh_var));
	if (!term->var)
		die("Malloc error");
}

int	main(void)
{
	t_input		input;
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
	//add_signals();
	while (1)
	{
		init_terminal(getenv("TERM"), term);
		init_input_and_cmd(&input, &cmd);
		take_input(&input);
		if (ft_strlen(input.line) > 0)
		{
			add_history(input.line);
			if (input.to_expand)
				try_expand(&input, term);
			split_command(input.line, &cmd);
			execute(&cmd, term);
		}
		free(input.line);
	}
	clear_history();
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
