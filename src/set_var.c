/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:40 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:58:10 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_into_vars(char *key, char *value, t_dict *where)
{
	t_dict	*new;
	t_dict	*tmp;

	if (!where)
		malloc_and_check_dict(&where, 1);
	new = where;
	tmp = where;
	if (new->key)
	{
		while (new)
		{
			tmp = new;
			new = new->next;
		}
		malloc_and_check_dict(&new, 1);
		tmp->next = new;
	}
	malloc_c(&new->key, ft_strlen(key) + 1);
	ft_strlcpy(new->key, key, ft_strlen(key) + 1);
	malloc_c(&new->value, ft_strlen(value) + 1);
	ft_strlcpy(new->value, value, ft_strlen(value) + 1);
	new->next = NULL;
}

int	change_exist_var_in_dict(char *key, char *value, t_dict *where)
{
	t_dict	*tmp;

	if (!where)
	{
		malloc_and_check_dict(&where, 1);
		tmp = where;
	}
	else
		tmp = where;
	while (tmp && tmp->key)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1))
		{
			free(tmp->value);
			malloc_c(&tmp->value, ft_strlen(value) + 1);
			ft_strlcpy(tmp->value, value, ft_strlen(value) + 1);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	sup_set_sh_var(int j, char *args, char *value, char *key)
{
	int	i;

	i = value_len(args);
	while (args[j] != '=')
		j++;
	while (args[j] == '\"' || args[j] == '\'')
		j++;
	if (quoted(args))
		ft_strlcpy(value, &args[j + 2], i + 1);
	else
		ft_strlcpy(value, &args[j + 1], i + 1);
	if (!change_exist_var_in_dict(key, value, g_term.env))
	{
		if (!change_exist_var_in_dict(key, value, g_term.var))
			insert_into_vars(key, value, g_term.var);
	}
	free(key);
	free(value);
}

void	set_sh_var(char **args)
{
	int		i;
	int		j;
	int		k;
	char	*key;
	char	*value;

	k = 0;
	key = NULL;
	value = NULL;
	while (args[k])
	{
		i = key_len(args[k]);
		if (i == 0)
			return ;
		malloc_c(&key, i + 1);
		ft_strlcpy(key, args[k], i + 1);
		j = i;
		i = value_len(args[k]);
		malloc_c(&value, i + 1);
		sup_set_sh_var(j, args[k], value, key);
		k++;
	}
}

void	treat_var_decl(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (!is_var_def(cmd->args[i]))
		{
			rewrite_args(cmd);
			execute_tree(cmd);
			return ;
		}
		i++;
	}
	set_sh_var(cmd->args);
	free_array_of_array(cmd->args);
	free(cmd->cmd);
}
