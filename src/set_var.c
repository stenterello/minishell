/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:40 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/03 14:44:02 by ddelladi         ###   ########.fr       */
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

int	var_to_append(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '=')
			return (0);
		else if (!ft_strncmp(&line[i], "+=", 2))
			return (1);
		i++;
	}
	return (0);
}

t_dict	*try_search(char *key, t_dict *where)
{
	t_dict	*tmp;

	if (!where)
		return (NULL);
	tmp = where;
	while (tmp && tmp->key)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_dict	*find_ptr(char *key)
{
	t_dict	*ptr;

	ptr = try_search(key, g_term.env);
	if (!ptr)
		ptr = try_search(key, g_term.var);
	if (!ptr)
		return (NULL);
	return (ptr);
}
