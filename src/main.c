/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:54:41 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/20 10:12:28 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execution_loop(t_terminfo *terminfo)
{
	t_command	cmd;

	init_cmd(&cmd);
	add_history(terminfo->input->line);
	if (terminfo->input->with_error)
		return ;
	while (terminfo->input->to_expand)
	{
		try_expand(terminfo);
		check(terminfo->input->line, terminfo);
	}
	cmd.first = 1;
	if (is_chained(terminfo->input->line))
		get_keys(terminfo->input->line, &cmd, terminfo);
	else if (split_command(terminfo->input->line, &cmd, terminfo))
		execute_tree(&cmd, terminfo);
	free(terminfo->input->line);
	terminfo->input->line = NULL;
	suspended_cat(terminfo);
	if (cmd.portions)
		free_array_of_array(cmd.portions);
}

void	init_and_take_input(t_terminfo *terminfo)
{
	while (1)
	{
		init_input(terminfo->input);
		transform_environ(terminfo);
		add_signals(terminfo, NULL);
		take_input(terminfo);
		if (ft_strlen(terminfo->input->line) > 0 && terminfo->delimiter == 0)
			execution_loop(terminfo);
		if (terminfo->input->line)
			free(terminfo->input->line);
		free_array_of_array(terminfo->glob_environ);
		terminfo->delimiter = 0;
		terminfo->suspended_cat = 0;
		terminfo->is_suspended = 1;
		terminfo->top = 0;
		terminfo->signaled = 0;
		g_child = 0;
	}
}

void	init_terminfo(t_terminfo *terminfo)
{
	terminfo->input = malloc(sizeof(t_input));
	if (!terminfo->input)
		die("Malloc error");
	terminfo->delimiter = 0;
	terminfo->suspended_cat = 0;
	terminfo->is_suspended = 0;
	terminfo->top = 0;
	terminfo->signaled = 0;
	terminfo->last_exit = 0;
	g_child = 0;
}

int	main(int argc, char **argv)
{
	t_terminfo	terminfo;

	init_terminfo(&terminfo);
	save_term(&terminfo.old_term);
	malloc_and_check_dict(&terminfo.var, 1);
	take_environ(&terminfo);
	init_terminal(ft_getenv("TERM", &terminfo), &terminfo);
	if (argc > 2 && !ft_strncmp("-c\0", argv[1], 3))
		return (c_run(argv, &terminfo));
	else if (argc > 1)
		return (script_run(argv, &terminfo));
	else
		init_and_take_input(&terminfo);
	clear_history();
	free_dict(terminfo.env);
	free_dict(terminfo.var);
	free(terminfo.new_term);
	free_array_of_array(terminfo.glob_environ);
	return (0);
}

/*

> out2 > out4 cat Makefile | grep all | wc -l > out5
Segnali
Verificare la dicitura: "Command not found" su bash
Premendo tab in un prompt vuoto, vengono presentate le varie opzioni di selezione: è questo il comportamento nella bash?
Al momento con sigquit il segnale inviato è un SIGINT perché con il SIGQUIT non viene terminato il processo (nella bash il SIGQUIT viene ignorato): verificare se è questo il comportamento da mantenere

*/
