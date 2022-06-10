/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 15:56:33 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/10 15:56:49 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	preliminary(t_command *tmp)
{
	t_command	*bench;

	bench = tmp->next;
	if (!tmp->cmd && !g_term.delimiter)
	{
		treat_var_decl(tmp);
		return (1);
	}
	else if (!tmp->cmd && tmp->args && ft_strchr(tmp->args[0], '=')
		&& tmp->args[0][0] != '=' && tmp->args[1])
		rewrite_args(tmp);
	else if (infinite_exit(tmp) && bench && !ft_strncmp(bench->cmd, "exit\0", 5))
	{
		free_commands(tmp);
		return (1);
	}
	return (0);
}
