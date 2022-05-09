/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:42 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 11:37:58 by ddelladi         ###   ########.fr       */
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
	g_term.glob_environ = NULL;
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

void	transform_environ(t_dict *env)
{
	t_dict	*tmp;
	int		i;

	i = 0;
	malloc_c_ptr(&g_term.glob_environ, env_size(env) + 1);
	tmp = env;
	while (tmp)
	{
		malloc_c(&g_term.glob_environ[i], ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2);
		ft_strlcpy(g_term.glob_environ[i], tmp->key, ft_strlen(tmp->key) + 1);
		ft_strlcat(g_term.glob_environ[i], "=", ft_strlen(g_term.glob_environ[i]) + 2);
		ft_strlcat(g_term.glob_environ[i], tmp->value, ft_strlen(g_term.glob_environ[i]) + ft_strlen(tmp->value) + 1);
		i++;
		tmp = tmp->next;
	}
	g_term.glob_environ[i] = NULL;
}

void	born_child(t_command *tmp)
{
	int			status;

	status = 0;
	if (tmp->to_pipe)
		define_pipe(tmp);
	if (tmp->to_pipe_to)
		define_pipe_to(tmp);
	g_term.child = fork();
	if (g_term.child == -1)
		die(strerror(errno));
	if (g_term.child == 0)
	{
		if (!ft_strncmp(&tmp->cmd[ft_strlen(tmp->cmd) - 9], "minishell", 9))
			next_level();
		execve(tmp->cmd, tmp->args, g_term.glob_environ);
	}
	else
		sup_born(tmp, status);
}

int	preliminary(t_command *tmp)
{
	if (!tmp->cmd && !g_term.delimiter)
	{
		treat_var_decl(tmp);
		return (1);
	}
	else if (!tmp->cmd && tmp->args && ft_strchr(tmp->args[0], '=')
		&& tmp->args[0][0] != '=' && tmp->args[1])
		rewrite_args(tmp);
	return (0);
}

void	free_commands(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i = 0;
		if (tmp->args)
		{
			while (tmp->args[i])
				free(tmp->args[i++]);
			free(tmp->args);
		}
		if (tmp->cmd)
			free(tmp->cmd);
		if (tmp->input_line)
			free(tmp->input_line);
		if (!tmp->first)
			free(tmp);
		tmp = tmp->next;
	}
	tmp = cmd;
}

void	execute_tree(t_command *cmd)
{
	t_command	*tmp;
	int			ret;

	tmp = cmd;
	if (preliminary(tmp))
		return ;
	ret = sup_ex(tmp);
	if (!ret)
		return ;
	if (!g_term.delimiter)
		free_commands(cmd);
}
