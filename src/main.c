/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:54:41 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/28 20:46:06 by ddelladi         ###   ########.fr       */
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
		if (terminfo->input->line && ft_strlen(terminfo->input->line) > 0
			&& terminfo->delimiter == 0)
			execution_loop(terminfo);
		if (terminfo->input->line)
		{
			free(terminfo->input->line);
			terminfo->input->line = NULL;
		}
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

- aggiustare segnali heredoc
- forse la raccolta di terminfo->input->line, per heredoc, deve essere presa dentro a standard execution (altrimenti la terminfo->input->line eventuale non è detto si riferisca al comando corrente, nel caso di più comandi di heredoc). Update: adesso lo fa ma è comunque da aggiustare, per esempio per quanto riguarda la history e l'output degli errori
- heredoc aggiorna la history!
- confrontare output di bash e minishell della stringa: cat ciao << eof | grep << foe

*/
