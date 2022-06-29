/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 16:35:00 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/29 11:56:35 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		g_child = 0;
	}
}
