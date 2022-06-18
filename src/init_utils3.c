/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 18:58:48 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/17 18:07:05 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	take_environ(t_terminfo *terminfo)
{
	int			i;
	t_dict		*new;
	t_dict		*prev;
	extern char	**environ;
	char		**tmp;

	i = 0;
	new = NULL;
	if (!terminfo->env)
		malloc_and_check_dict(&terminfo->env, 1);
	take_elem(terminfo->env, &i);
	prev = terminfo->env;
	tmp = environ;
	while (tmp[i])
	{
		malloc_and_check_dict(&new, 1);
		prev->next = new;
		prev = new;
		take_elem(new, &i);
	}
	change_exist_var_in_dict("SHELL", "minishell", terminfo->env);
}

int	env_size(t_dict *env)
{
	int		i;
	t_dict	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	sup_transform(t_dict *tmp, int i, t_terminfo *terminfo)
{
	if (tmp->value)
	{
		malloc_c(&terminfo->glob_environ[i], ft_strlen(tmp->key)
			+ ft_strlen(tmp->value) + 2);
		ft_strlcpy(terminfo->glob_environ[i], tmp->key,
			ft_strlen(tmp->key) + 1);
		ft_strlcat(terminfo->glob_environ[i], "=",
			ft_strlen(terminfo->glob_environ[i]) + 2);
		ft_strlcat(terminfo->glob_environ[i], tmp->value,
			ft_strlen(terminfo->glob_environ[i]) + ft_strlen(tmp->value) + 1);
	}
	else
	{
		malloc_c(&terminfo->glob_environ[i], ft_strlen(tmp->key) + 1);
		ft_strlcpy(terminfo->glob_environ[i], tmp->key,
			ft_strlen(tmp->key) + 1);
	}
}

void	transform_environ(t_terminfo *terminfo)
{
	t_dict	*tmp;
	int		i;

	i = 0;
	malloc_c_ptr(&terminfo->glob_environ, env_size(terminfo->env) + 1);
	tmp = terminfo->env;
	while (tmp)
	{
		sup_transform(tmp, i, terminfo);
		i++;
		tmp = tmp->next;
	}
	terminfo->glob_environ[i] = NULL;
}
