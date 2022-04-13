#include "minishell.h"

void	init_sh_var()
{
	g_term.var = malloc(sizeof(t_sh_var));
	if (!g_term.var)
		die("Malloc error");
	g_term.env = malloc(sizeof(t_env_var));
	if (!g_term.env)
		die("Malloc error");
}

int	main(void)
{
	t_command	cmd;

	init_sh_var();
	take_environ();
	
	while (1)
	{
		init_terminal(getenv("g_TERM"));
		init_input_and_cmd(&g_term.input, &cmd);
		add_signals();
		take_input(&g_term.input);
		if (ft_strlen(g_term.input.line) > 0)
		{
			add_history(g_term.input.line);
			while (g_term.input.to_expand)
			{
				try_expand(&g_term.input);
				check(g_term.input.line, &g_term.input);
			}
			split_command(g_term.input.line, &cmd);
			execute(&cmd);
		}
		free(g_term.input.line);
	}
	rl_clear_history();
	free_env(g_term.env);
	free_sh(g_term.var);
	free(g_term.termi);
	return (0);
}

/*

// aggiungi i segnali Ctrl-D Ctrl-C Ctrl-\ 

// implementa i redirezionamenti

// implementa le pipe

// implementa exit status

*/
