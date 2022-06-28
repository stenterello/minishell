/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 16:35:00 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/28 20:31:02 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_here(char *delimiter, int i, t_terminfo *terminfo)
{
	ft_putstr_fd(last_field(ft_getenv("SHELL", terminfo)), 2);
	ft_putstr_fd(": attention: here-document on line ", 2);
	ft_putnbr_fd(i, 2);
	ft_putstr_fd(" is delimited by an EOF (\"", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("\" was required)", 2);
}

void	take_heredoc_input(char *tmp, char *d, t_terminfo *terminfo)
{
	int	i;

	i = 1;
	if (terminfo->last_exit == 130)
		terminfo->last_exit = 0;
	tmp = readline("> ");
	while (terminfo->last_exit != 130 && tmp
		&& ft_strncmp(tmp, d, ft_strlen(d)) && g_child != -1)
	{
		if (terminfo->input->line)
			sup1_sup1(terminfo, tmp);
		else
		{
			malloc_c(&terminfo->input->line, ft_strlen(tmp) + 1);
			ft_strlcpy(terminfo->input->line, tmp, ft_strlen(tmp) + 1);
			ft_strlcat(terminfo->input->line, "\n", ft_strlen(terminfo->input->line) + 2);
		}
		free(tmp);
		tmp = NULL;
		i++;
		tmp = readline("> ");
		if (g_child == -1)
			break ;
	}
	if (tmp)
		free(tmp);
	end_take(tmp, i, d, terminfo);
}

int	to_exp(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_strncmp(&str[i], "<<", 2))
		i++;
	i += 2;
	while (str[i] == ' ')
		i++;
	if (str[i] == '\'')
		return (0);
	return (1);
}

int	to_expand_str(char *str)
{
	int		i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '$' && !is_open(str, i))
			return (1);
		i++;
	}
	return (0);
}

int	key_here_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isalnum(str[i]))
		i++;
	return (i);
}
