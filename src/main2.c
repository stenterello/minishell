/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:27:40 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 17:28:44 by gimartin         ###   ########.fr       */
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
