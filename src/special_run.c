/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_run.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 12:33:48 by ddelladi          #+#    #+#             */
/*   Updated: 2022/05/24 12:45:37 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	c_run(char **argv)
{
	int			i;
	t_command	cmd;

	i = 0;
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

void	fd_error(char *line)
{
	ft_putstr_fd(ft_getenv("SHELL"), 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(line, 2);
	ft_putstr_fd(": ", 2);
	die(strerror(errno));
}

void	read_and_execute(char *line, t_command *cmd)
{
	malloc_c(&g_term.input.line, ft_strlen(line) + 1);
	ft_strlcpy(g_term.input.line, line, ft_strlen(line));
	init_cmd(cmd);
	sup_loop(*cmd);
	free(g_term.input.line);
}

void	script_run(char **argv)
{
	int			i;
	int			fd;
	char		*line;
	t_command	cmd;

	i = 0;
	while (argv[i])
		i++;
	argv[i] = NULL;
	transform_environ(g_term.env);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		fd_error(argv[1]);
	line = get_next_line(fd);
	while (line)
	{
		if (ft_strncmp("#!/bin/bash", line, 11) && ft_strlen(line) > 1)
			read_and_execute(line, &cmd);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	reset_term();
	return ;
}

void	suspended_cat(void)
{
	int	i;

	i = 0;
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
