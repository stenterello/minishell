/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:14 by gimartin          #+#    #+#             */
/*   Updated: 2022/04/28 15:13:21 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	die(char *msg)
{
	ft_putendl_fd(msg, 2);
	exit(-1);
}

int	var_n_len(char *variable)
{
	int	i;

	i = 0;
	if (!ft_strncmp(variable, "?", 1) && !ft_isalnum(variable[i + 1]))
		return (1);
	while (variable[i] && ft_isalnum(variable[i]))
		i++;
	return (i);
}

char	*search_vars(char *var_name, t_dict *where)
{
	t_dict	*tmp;
	char	*ret;

	ret = NULL;
	tmp = where;
	while (tmp)
	{
		if (ft_strlen(tmp->key) == ft_strlen(var_name))
		{
			if (!ft_strncmp(tmp->key, var_name, ft_strlen(var_name)))
			{
				malloc_c(&ret, ft_strlen(tmp->value) + 1);
				ft_strlcpy(ret, tmp->value, ft_strlen(tmp->value) + 1);
				return (ret);
			}
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*sup_take(t_input *input, int init_len, char *var_name, char *var1)
{
	int		i;
	char	*ret;

	i = 0;
	while (var1[i] && (ft_isalnum(var1[i]) || var1[i] == '?'))
		i++;
	var_name[i] = '\0';
	malloc_c(&ret, ft_strlen(input->line) + 1 - i + ft_strlen(input->expanded));
	ft_strlcpy(ret, input->line, init_len + 1);
	ft_strlcpy(&ret[init_len], input->expanded, ft_strlen(input->expanded) + 1);
	if ((int)ft_strlen(input->expanded) > var_n_len(var1)
		&& input->line[var_n_len(var1) + 1 + init_len])
		ft_strlcpy(&ret[init_len + ft_strlen(input->expanded)],
			&input->line[init_len + var_n_len(var1) + 1],
			ft_strlen(input->line) - (init_len + var_n_len(var1)) + 1);
	else if (ft_strncmp(var_name,
			&input->line[var_n_len(var1) + 1 + init_len], 2))
		ft_strlcpy(&ret[init_len + ft_strlen(input->expanded)],
			&input->line[init_len + var_n_len(var1) + 1],
			ft_strlen(input->line) - (init_len + var_n_len(var1)) + 1);
	free(var_name);
	return (ret);
}

void	take_variable(char *var1, t_input *input, int init_len)
{
	int		i;
	char	*var_name;
	char	*ret;

	i = 0;
	var_name = NULL;
	malloc_c(&var_name, var_n_len(var1) + 1);
	while (var1[i] && (ft_isalnum(var1[i]) || var1[i] == '?'))
	{
		var_name[i] = var1[i];
		i++;
	}
	var_name[i] = '\0';
	if (!ft_strncmp(var_name, "?", 1))
		input->expanded = ft_itoa(g_term.last_exit);
	else
		input->expanded = search_vars(var_name, g_term.env);
	if (input->expanded == NULL)
		input->expanded = search_vars(var_name, g_term.var);
	if (input->expanded == NULL)
		input->expanded = ft_calloc(1, sizeof(char));
	ret = sup_take(input, init_len, var_name, var1);
	free(input->line);
	input->line = ret;
	free(input->expanded);
}
