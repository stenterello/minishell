/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:54:41 by ddelladi          #+#    #+#             */
/*   Updated: 2022/05/16 23:49:29 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	empty_redir(char *line)
{
	char	*tmp;

	tmp = ft_strtrim(line, " ()");
	if (tmp[0] == '>')
	{
		free(tmp);
		return (empty_output(line));
	}
	else if (tmp[0] == '<')
	{
		free(tmp);
		return (empty_input(line));
	}
	else if (no_output(line))
	{
		free(tmp);
		return (syntax_error_no_arr());
	}
	free(tmp);
	return (0);
}

void	sup_loop(t_command cmd)
{
	int	i;

	i = 0;
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
	while (i < g_term.suspended_cat)
	{
		g_term.input.line = readline("");
		if (!ft_strlen(g_term.input.line))
			ft_putchar_fd('\n', STDOUT_FILENO);
		free(g_term.input.line);
		g_term.input.line = NULL;
		i++;
	}
}

void	main_loop(int argc, char **argv)
{
	t_command	cmd;
	int			i;
	int			fd;
	char		*line;

	i = 0;
	if (argc > 2 && !ft_strncmp("-c\0", argv[1], 3))
	{
		while (argv[i])
			i++;
		argv[i] = NULL;
		transform_environ(g_term.env);
		malloc_c(&g_term.input.line, ft_strlen(argv[2]) + 1);
		ft_strlcpy(g_term.input.line, argv[2], ft_strlen(argv[2]) + 1);
		init_cmd(&cmd);
		sup_loop(cmd);
		free(g_term.input.line);
		reset_term();
		return ;
	}
	else if (argc > 1)
	{
		while (argv[i])
			i++;
		argv[i] = NULL;
		transform_environ(g_term.env);
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd(ft_getenv("SHELL"), 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd(": ", 2);
			die(strerror(errno));
		}
		line = get_next_line(fd);
		while (line)
		{
			if (ft_strncmp("#!/bin/bash", line, 11) && ft_strlen(line) > 1)
			{
				malloc_c(&g_term.input.line, ft_strlen(line) + 1);
				ft_strlcpy(g_term.input.line, line, ft_strlen(line));
				init_cmd(&cmd);
				sup_loop(cmd);
				free(g_term.input.line);
			}
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
		reset_term();
		return ;
	}
	else
	{
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
		}
	}
}

int	main(int argc, char **argv)
{
	save_term(&g_term.old_term);
	malloc_and_check_dict(&g_term.var, 1);
	take_environ();
	init_terminal(ft_getenv("TERM"));
	main_loop(argc, argv);
	clear_history();
	free_dict(g_term.env);
	free_dict(g_term.var);
	free(g_term.termi);
	free_array_of_array(g_term.glob_environ);
	return (0);
}