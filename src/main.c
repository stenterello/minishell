/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:54:41 by ddelladi          #+#    #+#             */
/*   Updated: 2022/05/04 15:23:23 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_filename(char *line)
{
	int		i;
	int		j;
	char	*ret;

	i = 1;
	while (!ft_isalnum(line[i]))
		i++;
	j = i;
	while (ft_isalnum(line[j]))
		j++;
	ret = NULL;
	malloc_c(&ret, j - i + 1);
	ft_strlcpy(ret, &line[i], j - i + 1);
	return (ret);
}

int	empty_output(char *line)
{
	char	*filename;
	int		fd;

	filename = get_filename(line);
	fd = open(filename, O_CREAT, 0664);
	if (fd < 0)
		die(strerror(errno));
	free(filename);
	close(fd);
	return (1);
}

int	empty_input(char *line)
{
	char	*filename;
	int		fd;

	filename = get_filename(line);
	fd = open(filename, O_CREAT, 0664);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_term.last_exit = 1;
	}
	free(filename);
	close(fd);
	return (1);
}

int	syntax_error_no_arr(void)
{
	ft_putstr_fd(last_field(ft_getenv("SHELL")), 2);
	ft_putendl_fd(": syntax error near unexpected token \"newline\"", 2);
	return (1);
}

int	no_output(char *line)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (line[i])
	{
		if (is_token(&line[i]) && !flag)
			flag = 1;
		if (ft_isalnum(line[i]) && flag)
			flag = 0;
		i++;
	}
	if (flag)
		return (1);
	return (0);
}

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
	return (0);
}

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
	if (empty_redir(g_term.input.line))
		return ;
	else if (is_logical(g_term.input.line))
		get_logical(g_term.input.line, &cmd);
	else if (split_command(g_term.input.line, &cmd))
		execute_tree(&cmd);
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
		free_array_of_array(g_term.glob_environ);
		g_term.delimiter = 0;
	}
}

int	main(void)
{
	save_term(&g_term.old_term);
	malloc_and_check_dict(&g_term.var, 1);
	take_environ();
	init_terminal(ft_getenv("TERM"));
	main_loop();
	clear_history();
	free_dict(g_term.env);
	free_dict(g_term.var);
	free(g_term.termi);
	free(g_term.glob_environ);
	return (0);
}

/*

// DA GESTIRE IL SEGNALE CTRL + \

// La free a riga 129 di heredoc.c non dovrebbe dare invalid
	pointer nel caso in cui venga premuto Ctrl + D 
	durante una readline?

// Da controllare:
	- quali segnali, di preciso, servono
	- leaks
	- norma
	- configurazioni (flag) del terminale

*/
