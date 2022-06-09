/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:42 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/06 15:00:27 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rewrite_args(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (is_var_def(cmd->args[i]))
		i++;
	malloc_c(&cmd->cmd, ft_strlen(cmd->args[i]) + 1);
	ft_strlcpy(cmd->cmd, cmd->args[i], ft_strlen(cmd->args[i]) + 1);
	while (cmd->args[i])
	{
		free(cmd->args[j]);
		malloc_c(&cmd->args[j], ft_strlen(cmd->args[i]) + 1);
		ft_strlcpy(cmd->args[j], cmd->args[i], ft_strlen(cmd->args[i]) + 1);
		free(cmd->args[i]);
		cmd->args[i] = NULL;
		i++;
		j++;
	}
	while (cmd->args[j])
	{
		free(cmd->args[j]);
		cmd->args[j++] = NULL;
	}
}

void	next_level(void)
{
	char	*bef;

	bef = ft_itoa((ft_atoi(ft_getenv("SHLVL")) + 1));
	change_exist_var_in_dict("SHLVL", bef, g_term.env);
	free_array_of_array(g_term.glob_environ);
	transform_environ(g_term.env);
	free(bef);
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

void	sup_transform(t_dict *tmp, int i)
{
	if (tmp->value)
	{
		malloc_c(&g_term.glob_environ[i], ft_strlen(tmp->key)
			+ ft_strlen(tmp->value) + 2);
		ft_strlcpy(g_term.glob_environ[i], tmp->key,
			ft_strlen(tmp->key) + 1);
		ft_strlcat(g_term.glob_environ[i], "=",
			ft_strlen(g_term.glob_environ[i]) + 2);
		ft_strlcat(g_term.glob_environ[i], tmp->value,
			ft_strlen(g_term.glob_environ[i]) + ft_strlen(tmp->value) + 1);
	}
	else
	{
		malloc_c(&g_term.glob_environ[i], ft_strlen(tmp->key) + 1);
		ft_strlcpy(g_term.glob_environ[i], tmp->key,
			ft_strlen(tmp->key) + 1);
	}
}

void	transform_environ(t_dict *env)
{
	t_dict	*tmp;
	int		i;

	i = 0;
	malloc_c_ptr(&g_term.glob_environ, env_size(env) + 1);
	tmp = env;
	while (tmp)
	{
		sup_transform(tmp, i);
		i++;
		tmp = tmp->next;
	}
	g_term.glob_environ[i] = NULL;
}
