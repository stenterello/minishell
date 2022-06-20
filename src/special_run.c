/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_run.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 12:33:48 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/20 10:39:54 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	c_run(char **argv, t_terminfo *terminfo)
{
	int		i;
	char	*help;

	i = 0;
	while (argv[i])
		i++;
	argv[i] = NULL;
	transform_environ(terminfo);
	i = 2;
	while (argv[i])
	{
		malloc_c(&help, ft_strlen(terminfo->input->line) + ft_strlen(argv[i]) + 1);
		ft_strlcpy(help, terminfo->input->line, ft_strlen(terminfo->input->line) + 1);
		ft_strlcat(help, " ", ft_strlen(help) + 2);
		ft_strlcat(help, argv[i], ft_strlen(help) + ft_strlen(argv[i]) + 1);
		if (terminfo->input->line)
			free(terminfo->input->line);
		malloc_c(&terminfo->input->line, ft_strlen(help) + 1);
		ft_strlcpy(terminfo->input->line, help, ft_strlen(help) + 1);
		free(help);
		i++;
	}
	execution_loop(terminfo);
	free(terminfo->input->line);
	reset_term(terminfo);
	return (0);
}

void	fd_error(char *line, t_terminfo *terminfo)
{
	ft_putstr_fd(ft_getenv("SHELL", terminfo), 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(line, 2);
	ft_putstr_fd(": ", 2);
	die(strerror(errno));
}

void	read_and_execute(char *line, t_terminfo *terminfo)
{
	malloc_c(&terminfo->input->line, ft_strlen(line) + 1);
	ft_strlcpy(terminfo->input->line, line, ft_strlen(line));
	execution_loop(terminfo);
	free(terminfo->input->line);
}

int	script_run(char **argv, t_terminfo *terminfo)
{
	int			i;
	int			fd;
	char		*line;

	i = 0;
	while (argv[i])
		i++;
	argv[i] = NULL;
	transform_environ(terminfo);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		fd_error(argv[1], terminfo);
	line = get_next_line(fd);
	while (line)
	{
		if (ft_strncmp("#!/bin/bash", line, 11) && ft_strlen(line) > 1)
			read_and_execute(line, terminfo);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	reset_term(terminfo);
	return (0);
}

void	suspended_cat(t_terminfo *terminfo)
{
	int	i;

	i = 0;
	while (i < terminfo->suspended_cat)
	{
		terminfo->input->line = readline("");
		if (!ft_strlen(terminfo->input->line))
			ft_putchar_fd('\n', STDOUT_FILENO);
		free(terminfo->input->line);
		terminfo->input->line = NULL;
		i++;
	}
}
