/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:30 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/03 15:10:17 by ddelladi         ###   ########.fr       */
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

t_dict	*try_search_env(char *key)
{
	t_dict	*tmp;

	tmp = g_term.env;
	if (!tmp)
		return (NULL);
	while (tmp && tmp->key)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	expand_if_sup_exp(t_dict *new, t_command *cmd)
{
	new->value = NULL;
	new->value = search_vars(cmd->args[1], g_term.var);
	if (!new->value)
	{
		free(new->key);
		free(new);
		g_term.last_exit = 0;
		return (0);
	}
	return (1);
}

int	extend_sup_exp(t_dict *new, int i, t_command *cmd, int j)
{
	t_dict	*append_ptr;
	char	*new_string;

	malloc_c(&new->value, i + 1);
	ft_strlcpy(new->value, &cmd->args[1][j + 1], i + 1);
	append_ptr = try_search_env(new->key);
	if (append_ptr)
	{
		malloc_c(&new_string, ft_strlen(append_ptr->value)
			+ ft_strlen(new->value) + 1);
		ft_strlcpy(new_string, append_ptr->value,
			ft_strlen(append_ptr->value) + 1);
		ft_strlcat(new_string, new->value, ft_strlen(new_string)
			+ ft_strlen(new->value) + 1);
		free(append_ptr->value);
		append_ptr->value = new_string;
		free(new->key);
		free(new->value);
		free(new);
		return (1);
	}
	return (0);
}

void	sup_export(t_command *cmd, t_dict *new, int i)
{
	int		j;

	j = 0;
	while (cmd->args[1][j] && cmd->args[1][j] != '='
		&& ft_strncmp(&cmd->args[1][j], "+=", 2))
		j++;
	if (!ft_strncmp(&cmd->args[1][j], "+=", 2))
		j++;
	if (!cmd->args[1][j])
	{
		if (!expand_if_sup_exp(new, cmd))
			return ;
	}
	else
	{
		if (extend_sup_exp(new, i, cmd, j))
			return ;
	}
	new->next = NULL;
	sup_sup_export(new);
}
