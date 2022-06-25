/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 12:13:03 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/24 17:36:49 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*take_key(char *str)
{
	int		i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (str[i])
	{
		if (str[i] == '$' && !is_open(str, i))
		{
			i++;
			malloc_c(&ret, key_here_len(&str[i]) + 1);
			ft_strlcpy(ret, &str[i], key_here_len(&str[i]) + 1);
			return (ret);
		}
		i++;
	}
	return (ret);
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
