#include "minishell.h"

int	main(void)
{
	t_command	cmd;

	malloc_and_check_dict(&g_term.env, 1);
	malloc_and_check_dict(&g_term.var, 1);
	take_environ();
	
	while (1)
	{
		init_terminal(ft_getenv("TERM"));
		init_input(&g_term.input);
		init_cmd(&cmd);
		add_signals();
		take_input(&g_term.input);
		if (ft_strlen(g_term.input.line) > 0)
		{
			if (!is_heredoc(g_term.input.line))
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
	clear_history();
	free_dict(g_term.env);
	free_dict(g_term.var);
	free(g_term.termi);
	return (0);
}

/*

// aggiungi i segnali Ctrl-D Ctrl-C Ctrl-\ 

// implementa le pipe

*/
