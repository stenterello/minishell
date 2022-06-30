/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:16:53 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/30 16:31:59 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	delimiter_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != 32 && ft_strncmp(&line[i], "<<", 2))
		i++;
	return (i);
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

int	key_here_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i])
			|| str[i] == '?' || str[i] == '$'))
		i++;
	return (i);
}

int	ft_strlen_rl(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (++i);
		i++;
	}
	return (i);
}

void	end_take(char *tmp, int i, char *d, t_terminfo *terminfo)
{
	if (!tmp)
		print_here(d, i, terminfo);
	if (g_child == -1)
	{
		terminfo->last_exit = 130;
	}
}
