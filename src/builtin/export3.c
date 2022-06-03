/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 14:55:14 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/03 18:40:43 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export2(t_command *cmd, int i, t_dict *new)
{
	char	*ret;

	if (i == -1)
	{
		ret = search_vars(cmd->args[1], g_term.var);
		if (ret != NULL)
		{
			i = value_len(ret);
			free(ret);
		}
	}
	sup_export(cmd, new, i);
}

void	print_exported_env(void)
{
	t_dict	*tmp;

	tmp = g_term.env;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(tmp->key, STDOUT_FILENO);
		if (tmp->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
			ft_putchar_fd('\n', STDOUT_FILENO);
		tmp = tmp->next;
	}
}

void	export(t_command *cmd)
{
	int		i;
	t_dict	*new;

	new = NULL;
	i = 0;
	if (cmd->args[1])
	{
		if (check_export(cmd))
			return ;
		malloc_and_check_dict(&new, 1);
		i = key_len(cmd->args[1]);
		if (i == -1)
		{
			free(new);
			return ;
		}
		malloc_c(&new->key, i + 1);
		ft_strlcpy(new->key, cmd->args[1], i + 1);
		i = value_len(cmd->args[1]);
		export2(cmd, i, new);
	}
	else
		print_exported_env();
}
