/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:58 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/21 12:16:14 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_builtin(t_command *cmd, t_terminfo *terminfo)
{
	char	*ret;

	ret = pwd(terminfo);
	if (cmd->to_pipe)
		define_pipe(cmd);
	if (cmd->to_pipe_to)
		define_pipe_to(cmd);
	ft_putendl_fd(ret, 1);
	free(ret);
}

int	builtin(t_command *cmd, t_terminfo *terminfo)
{
	if (!cmd->cmd)
		return (0);
	if (!ft_strncmp(cmd->cmd, "pwd\0", 4))
		sup_builtin(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "cd\0", 3))
		cd(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "echo\0", 5))
		echo(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "exit\0", 5))
		exit_cmd(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "env\0", 4))
		env(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "export\0", 7))
		export(cmd, terminfo);
	else if (!ft_strncmp(cmd->cmd, "unset\0", 6))
		unset(cmd, terminfo);
	else
		return (0);
	if (cmd->to_pipe || cmd->to_pipe_to || cmd->redir_in || cmd->redir_out)
		restore_fd(cmd, terminfo);
	return (1);
}

int	other_cmds(char **tmp, int *c)
{
	int	i;

	i = 0;
	while (tmp[i])
	{
		if (!ft_strncmp(tmp[i], "|\0", 2))
		{
			c[0] += i;
			return (1);
		}
		i++;
	}
	return (0);
}

void	sup_sup_check(char *typed, int i, t_terminfo *terminfo, int *open)
{
	if (typed[i] == '\'' && !terminfo->input->s_quot && !terminfo->input->d_quot)
		terminfo->input->s_quot = 1;
	else if (typed[i] == '\'' && terminfo->input->s_quot && !terminfo->input->d_quot)
		terminfo->input->s_quot = 0;
	else if (typed[i] == '$' && !terminfo->input->s_quot && typed[i + 1] && (typed[i + 1] == '?' || typed[i + 1] == '$' || env_exists(&typed[i + 1], terminfo)))
		terminfo->input->to_expand = 1;
	else if (typed[i] == '\"' && !terminfo->input->d_quot && !terminfo->input->s_quot)
		terminfo->input->d_quot = 1;
	else if (typed[i] == '\"' && terminfo->input->d_quot && !terminfo->input->s_quot)
		terminfo->input->d_quot = 0;
	else if (typed[i] == '|' && !terminfo->input->d_quot && !terminfo->input->s_quot)
		open[0] = 1;
	else if (ft_isalnum(typed[i]) && open[0])
		open[0] = 0;
	else if (!open[1] && (!ft_strncmp(&typed[i], "&&", 2)
			|| !ft_strncmp(&typed[i], "||", 2)))
		open[1] = 1;
	else if (typed[i] == '(' && !is_open(typed, i))
		open[2]++;
	else if (typed[i] == ')' && !is_open(typed, i))
		open[3]++;
	if (ft_isalnum(typed[i]) && open[1])
		open[1] = 0;
}

int	sup_check(char *typed, int i, t_terminfo *terminfo, int *open)
{
	while (typed[i])
	{
		sup_sup_check(typed, i, terminfo, open);
		i++;
	}
	return (i);
}
