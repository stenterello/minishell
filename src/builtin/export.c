/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:30 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/11 17:59:14 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sup_sup_export(t_dict *new)
{
	if (!change_exist_var_in_dict(new->key, new->value, g_term.env))
		insert_into_vars(new->key, new->value, g_term.env);
	free(new->key);
	free(new->value);
	free(new);
	g_term.last_exit = 0;
}

void	sup_export(t_command *cmd, t_dict *new, int i)
{
	int		j;

	j = 0;
	while (cmd->args[1][j] && cmd->args[1][j] != '=')
		j++;
	if (!cmd->args[1][j])
	{
		new->value = NULL;
		new->value = search_vars(cmd->args[1], g_term.var);
		if (!new->value)
		{
			free(new->key);
			free(new);
			g_term.last_exit = 0;
			return ;
		}
	}
	else
	{
		malloc_c(&new->value, i + 1);
		ft_strlcpy(new->value, &cmd->args[1][j + 1], i + 1);
	}
	new->next = NULL;
	sup_sup_export(new);
}

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
		else
		{
			free(new->key);
			free(new);
			return ;
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
		// AGGIUNGERE CONTROLLI SULL'ESISTENZA DI tmp->key E tmp->value
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(tmp->key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(tmp->value, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
		tmp = tmp->next;
	}
}

void	export(t_command *cmd)
{
	int		i;
	t_dict	*new;

	new = NULL;
	if (cmd->args[1])
	{
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
