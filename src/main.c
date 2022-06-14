/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:54:41 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/14 14:42:59 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_loop(t_command cmd)
{
	add_history(g_term.input.line);
	if (g_term.input.with_error)
		return ;
	while (g_term.input.to_expand)
	{
		try_expand(&g_term.input);
		check(g_term.input.line, &g_term.input);
	}
	cmd.first = 1;
	if (is_logical(g_term.input.line))
		get_logical(g_term.input.line, &cmd);
	else if (split_command(g_term.input.line, &cmd))
		execute_tree(&cmd);
	free(g_term.input.line);
	g_term.input.line = NULL;
	suspended_cat();
}

void	main_loop(void)
{
	t_command	cmd;

	while (1)
	{
		init_input(&g_term.input);
		init_cmd(&cmd);
		transform_environ(g_term.env);
		add_signals();
		take_input(&g_term.input);
		if (ft_strlen(g_term.input.line) > 0 && g_term.delimiter == 0)
			sup_loop(cmd);
		if (g_term.input.line)
			free(g_term.input.line);
		if (cmd.portions)
			free_array_of_array(cmd.portions);
		free_array_of_array(g_term.glob_environ);
		g_term.delimiter = 0;
		g_term.suspended_cat = 0;
		g_term.is_suspended = 1;
		g_term.top = 0;
		g_term.signaled = 0;
	}
}

int	main(int argc, char **argv)
{
	save_term(&g_term.old_term);
	malloc_and_check_dict(&g_term.var, 1);
	take_environ();
	init_terminal(ft_getenv("TERM"));
	if (argc > 2 && !ft_strncmp("-c\0", argv[1], 3))
	{
		c_run(argv);
		return (0);
	}
	else if (argc > 1)
	{
		script_run(argv);
		return (0);
	}
	else
		main_loop();
	clear_history();
	free_dict(g_term.env);
	free_dict(g_term.var);
	free(g_term.termi);
	free_array_of_array(g_term.glob_environ);
	return (0);
}

/* 

Copiare backup heredoc
Esportazioni delle variabili malfunzionanti
> out2 > out4 cat Makefile | grep all | wc -l > out5
Segnali
Exit status quando si chiude il terminale

*/
