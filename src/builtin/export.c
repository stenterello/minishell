/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:30 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/13 15:45:33 by gimartin         ###   ########.fr       */
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

void	sup_export(t_command *cmd, t_dict *new, int i)
{
	int		j;
	t_dict	*append_ptr;
	char	*new_string;

	j = 0;
	while (cmd->args[1][j] && cmd->args[1][j] != '=' && ft_strncmp(&cmd->args[1][j], "+=", 2))
		j++;
	if (!ft_strncmp(&cmd->args[1][j], "+=", 2))
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
		append_ptr = try_search_env(new->key);
		if (append_ptr)
		{
			malloc_c(&new_string, ft_strlen(append_ptr->value) + ft_strlen(new->value) + 1);
			ft_strlcpy(new_string, append_ptr->value, ft_strlen(append_ptr->value) + 1);
			ft_strlcat(new_string, new->value, ft_strlen(new_string) + ft_strlen(new->value) + 1);
			free(append_ptr->value);
			append_ptr->value = new_string;
			free(new->key);
			free(new->value);
			free(new);
			return ;
		}
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
		ft_putstr_fd("=\"", STDOUT_FILENO);
		if (tmp->value)
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
