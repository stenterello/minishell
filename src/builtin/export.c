/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:30 by gimartin          #+#    #+#             */
/*   Updated: 2022/07/01 17:16:05 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_exported_env(t_terminfo *terminfo)
{
	t_dict	*tmp;

	tmp = terminfo->env;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(tmp->key, STDOUT_FILENO);
		if (tmp->value)
		{
			if (tmp->value[0] == '\"')
				ft_putstr_fd("=", STDOUT_FILENO);
			else
				ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			if (tmp->value[0] == '\"')
				ft_putstr_fd("\n", STDOUT_FILENO);
			else
				ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
			ft_putchar_fd('\n', STDOUT_FILENO);
		tmp = tmp->next;
	}
}

int	concatenate_value(t_dict *new, int i[2],
	t_command *cmd, t_terminfo *terminfo)
{
	t_dict	*append_ptr;
	char	*new_string;

	new_string = NULL;
	malloc_c(&new->value, i[0] + 1);
	ft_strlcpy(new->value, &cmd->args[1][i[1] + 1], i[0] + 1);
	append_ptr = try_search(new->key, terminfo->env);
	if (append_ptr)
	{
		save_concatenation(new_string, append_ptr->value,
			new->value, append_ptr);
		free_dict(new);
		return (1);
	}
	else
	{
		append_ptr = try_search(new->key, terminfo->var);
		save_concatenation(new_string, append_ptr->value, new->value, new);
		insert_into_vars(new->key, new->value, terminfo->env);
	}
	return (0);
}

void	understand_then_export(t_command *cmd, t_dict *new,
	int i[3], t_terminfo *terminfo)
{
	i[1] = equal_char_index(cmd, i[2]);
	if (!cmd->args[i[2]][i[1] + 1] && !var_exists(new, cmd, terminfo))
	{
		insert_empty_var(new, terminfo);
		return ;
	}
	else if (!ft_strncmp(&cmd->args[i[2]][i[1] - 1], "+=", 2)
		&& concatenate_value(new, i, cmd, terminfo))
		return ;
	else if (i[1] != (int)ft_strlen(cmd->args[i[2]]))
	{
		i[1]++;
		malloc_c(&new->value, ft_strlen(&cmd->args[i[2]][i[1]]));
		if (cmd->args[i[2]][i[1]] == '\"' || cmd->args[i[2]][i[1]] == '\'')
		{
			i[1]++;
			ft_strlcpy(new->value, &cmd->args[i[2]][i[1]],
				ft_strlen(&cmd->args[i[2]][i[1]]));
		}
		else
			ft_strlcpy(new->value, &cmd->args[i[2]][i[1]],
				ft_strlen(&cmd->args[i[2]][i[1]]) + 1);
	}
	new->next = NULL;
	export_to_env_vars(new, terminfo);
}

void	export_loop(t_command *cmd, t_terminfo *terminfo)
{
	t_dict	*new;
	int		i[3];

	new = NULL;
	ft_bzero((void *)i, 8);
	i[2] = 1;
	while (cmd->args[i[2]])
	{
		if (check_export_syntax(cmd, i[2], terminfo))
			return ;
		malloc_and_check_dict(&new, 1);
		i[0] = key_len(cmd->args[i[2]]);
		if (i[0] == -1)
		{
			free(new);
			return ;
		}
		malloc_c(&new->key, i[0] + 1);
		ft_strlcpy(new->key, cmd->args[i[2]], i[0] + 1);
		i[0] = value_len(cmd->args[i[2]]);
		search_value_for_key(cmd, &i[0], i[2], terminfo);
		understand_then_export(cmd, new, i, terminfo);
		i[2]++;
	}
}

void	export(t_command *cmd, t_terminfo *terminfo)
{
	if (cmd->args[1])
		export_loop(cmd, terminfo);
	else
		print_exported_env(terminfo);
}
