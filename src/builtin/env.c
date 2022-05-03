/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:23 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:55:45 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv(char *line)
{
	t_dict	*tmp;
	t_dict	*tmp2;

	if (g_term.env)
	{
		tmp = g_term.env;
		while (tmp)
		{
			if (!ft_strncmp(tmp->key, line, ft_strlen(line) + 1))
				return (tmp->value);
			tmp = tmp->next;
		}
	}
	if (g_term.var && g_term.var->key)
	{
		tmp2 = g_term.var;
		while (tmp2)
		{
			if (!ft_strncmp(tmp2->key, line, ft_strlen(line) + 1))
				return (tmp2->value);
			tmp2 = tmp2->next;
		}
	}
	return (NULL);
}

void	env(t_command *cmd)
{
	t_dict	*tmp;

	if (cmd->to_pipe)
		define_pipe(cmd);
	if (cmd->to_pipe_to)
		define_pipe_to(cmd);
	tmp = g_term.env;
	while (tmp)
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(tmp->value, 1);
		tmp = tmp->next;
	}
	g_term.last_exit = 0;
}
