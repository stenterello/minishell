/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 14:55:14 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/09 12:55:20 by gimartin         ###   ########.fr       */
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

int	control_j(t_command *cmd)
{
	int	j;

	j = 0;
	while (cmd->args[1][j] && cmd->args[1][j] != '='
		&& ft_strncmp(&cmd->args[1][j], "+=", 2))
		j++;
	if (!ft_strncmp(&cmd->args[1][j], "+=", 2))
		j++;
	return (j);
}

void	sup_export(t_command *cmd, t_dict *new, int i)
{
	int		j;

	j = control_j(cmd);
	if (!cmd->args[1][j])
	{
		if (!expand_if_sup_exp(new, cmd))
		{
			insert_empty_var(new);
			return ;
		}
	}
	else if (!ft_strncmp(&cmd->args[1][j - 2], "+=", 2))
	{
		if (extend_sup_exp(new, i, cmd, j))
			return ;
	}
	else
	{
		j++;
		malloc_c(&new->value, ft_strlen(&cmd->args[1][j]));
		ft_strlcpy(new->value, &cmd->args[1][j],
			ft_strlen(&cmd->args[1][j]) + 1);
	}
	new->next = NULL;
	sup_sup_export(new);
}
