/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_var2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 14:43:19 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/15 14:39:53 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_var(char *key, char *value, t_terminfo *terminfo)
{
	t_dict	*append_ptr;
	char	*new;
	int		len;

	append_ptr = find_ptr(key, terminfo);
	if (!append_ptr)
	{
		if (!change_exist_var_in_dict(key, value, terminfo->env))
		{
			if (!change_exist_var_in_dict(key, value, terminfo->var))
				insert_into_vars(key, value, terminfo->var);
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

void	insert_sh_var(int j, char *args, t_dict *new, t_terminfo *terminfo)
{
	int	i;

	i = value_len(args);
	while (args[j] != '=')
		j++;
	while (args[j] == '\"' || args[j] == '\'')
		j++;
	if (quoted(args))
		ft_strlcpy(new->value, &args[j + 2], i + 1);
	else
		ft_strlcpy(new->value, &args[j + 1], i + 1);
	if (!var_to_append(args))
	{
		if (!change_exist_var_in_dict(new->key, new->value, terminfo->env))
		{
			if (!change_exist_var_in_dict(new->key, new->value, terminfo->var))
				insert_into_vars(new->key, new->value, terminfo->var);
		}
	}
	else
		append_var(new->key, new->value, terminfo);
	free(new->key);
	free(new->value);
}

void	set_sh_var(char **args, t_terminfo *terminfo)
{
	int		i;
	int		j;
	int		k;
	t_dict	*new;

	k = 0;
	malloc_and_check_dict(&new, 1);
	new->key = NULL;
	new->value = NULL;
	while (args[k])
	{
		i = key_len(args[k]);
		if (i == 0)
			return ;
		malloc_c(&new->key, i + 1);
		ft_strlcpy(new->key, args[k], i + 1);
		j = i;
		i = value_len(args[k]);
		malloc_c(&new->value, i + 1);
		insert_sh_var(j, args[k], new, terminfo);
		k++;
	}
	free(new);
}

void	treat_var_decl(t_command *cmd, t_terminfo *terminfo)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (!is_var_def(cmd->args[i]))
		{
			rewrite_args(cmd);
			execute_tree(cmd, terminfo);
			return ;
		}
		i++;
	}
	set_sh_var(cmd->args, terminfo);
	free_array_of_array(cmd->args);
	free(cmd->cmd);
}
