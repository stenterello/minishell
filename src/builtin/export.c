/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:30 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:55:55 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	key_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	if (!line[i] && search_vars(line, g_term.var) == NULL)
		return (-1);
	return (i);
}

int	value_len(char *line)
{
	int	i;
	int	j;

	i = key_len(line) + 1;
	if (line[i] == ' ')
		return (0);
	while (line[i] == '\"' || line[i] == '\'')
		i++;
	j = i;
	while (line[i] && line[i] != '"' && line[i] != '\'')
		i++;
	if (i - j == 0)
		return (-1);
	return (i - j);
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
	if (!change_exist_var_in_dict(new->key, new->value, g_term.env))
		insert_into_vars(new->key, new->value, g_term.env);
	free(new->key);
	free(new->value);
	free(new);
	g_term.last_exit = 0;
}

void	export(t_command *cmd)
{
	int		i;
	char	*ret;
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
}
