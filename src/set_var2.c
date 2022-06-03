/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_var2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 14:43:19 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/03 14:43:55 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_var(char *key, char *value)
{
	t_dict	*append_ptr;
	char	*new;
	int		len;

	append_ptr = find_ptr(key);
	if (!append_ptr)
	{
		if (!change_exist_var_in_dict(key, value, g_term.env))
		{
			if (!change_exist_var_in_dict(key, value, g_term.var))
				insert_into_vars(key, value, g_term.var);
		}
	}
	else
	{
		len = ft_strlen(append_ptr->value) + ft_strlen(value) + 1;
		malloc_c(&new, len);
		ft_strlcpy(new, append_ptr->value, ft_strlen(append_ptr->value) + 1);
		ft_strlcat(new, value, len);
		free(append_ptr->value);
		append_ptr->value = new;
	}
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
	if (!var_to_append(args))
	{
		if (!change_exist_var_in_dict(key, value, g_term.env))
		{
			if (!change_exist_var_in_dict(key, value, g_term.var))
				insert_into_vars(key, value, g_term.var);
		}
	}
	else
		append_var(key, value);
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
