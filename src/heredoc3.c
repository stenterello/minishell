/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 16:35:00 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/15 17:02:13 by ddelladi         ###   ########.fr       */
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

char	*take_heredoc_input(char *tmp, char *d,
	t_command *cmd, t_terminfo *terminfo)
{
	int	i;

	i = 1;
	if (terminfo->last_exit == 130)
		terminfo->last_exit = 0;
	tmp = readline("> ");
	while (terminfo->last_exit != 130 && tmp && ft_strncmp(tmp, d, ft_strlen(d)))
	{
		if (cmd->input_line)
			sup1_sup1(cmd, tmp);
		else
		{
			malloc_c(&cmd->input_line, ft_strlen(tmp) + 1);
			ft_strlcpy(cmd->input_line, tmp, ft_strlen(tmp) + 1);
			ft_strlcat(cmd->input_line, "\n", ft_strlen(cmd->input_line) + 2);
		}
		free(tmp);
		i++;
		tmp = readline("> ");
		// if (terminfo->signaled)
		// 	break ;
	}
	if (!tmp)
		print_here(d, i, terminfo);
	return (tmp);
}

int	to_exp(char *str)
{
	int	i;

	i = 0;
	while (ft_strncmp(&str[i], "<<", 2))
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
