/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 12:13:03 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/26 18:44:49 by ddelladi         ###   ########.fr       */
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

int	init_heredoc(char *typed, t_terminfo *terminfo)
{
	t_command	*cmd;
	t_command	*cmd2;

	terminfo->delimiter = 1;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		die("Malloc error");
	cmd2 = malloc(sizeof(t_command));
	if (!cmd2)
		die("Malloc error");
	init_cmd(cmd);
	init_cmd(cmd2);
	cmd->first = 1;
	cmd2->next = cmd;
	treat_heredoc(cmd, cmd2, typed, terminfo);
	return (1);
}
