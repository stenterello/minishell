/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 15:51:58 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/14 18:24:06 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cpy_and_slide(char **tmp, int *c, int start, t_command *cmd)
{
	int	i;

	i = 0;
	start = 0;
	while (tmp[i] && !is_token(tmp[i]))
	{
		if (i == start)
		{
			malloc_c(&cmd->cmd, next_arg_len(tmp[i]) + 2);
			ft_strlcpy(cmd->cmd, tmp[i], next_arg_len(tmp[i]) + 1);
		}
		if (i != 0 && !ft_strncmp(cmd->cmd, "sed\0", 4))
			tmp[i] = ft_strtrim(tmp[i], "\'");
		malloc_c(&cmd->args[c[1]], ft_strlen(tmp[i]) + 1);
		ft_strlcpy(cmd->args[c[1]], tmp[i], ft_strlen(tmp[i]) + 1);
		i++;
		c[1]++;
	}
	c[0] = i;
	cmd->args[c[1]] = NULL;
}

void	fill_prev(t_command *cmd, int *c, char **tmp)
{
	t_command	*prev;

	if (cmd->redir_in || cmd->redir_out)
		c[0] += 2;
	if (tmp[c[0]] && tmp[c[0]][0] == '|')
		cmd->to_pipe = 1;
	cmd->args[c[1]] = NULL;
	if (cmd->prev)
	{
		prev = cmd->prev;
		if (prev->to_pipe)
			cmd->to_pipe_to = 1;
	}
}

void	fill_next(t_command *cmd, int *c, char **tmp, t_terminfo *terminfo)
{
	t_command	*next;

	if (tmp[c[0]] && (cmd->to_pipe || cmd->to_pipe_to))
	{
		next = malloc(sizeof(t_command));
		if (!next)
			die("Malloc error");
		init_cmd(next);
		cmd->next = next;
		next->prev = cmd;
		++c[0];
		fill_cmd_fields(tmp, next, c[0], terminfo);
	}
}

int	count_cleaned_cmd(char **tmp)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (tmp[i] && ft_strncmp(tmp[i], "|\0", 2))
	{
		if (!ft_strncmp(tmp[i], ">\0", 2) || !ft_strncmp(tmp[i], ">>\0", 3)
			|| !ft_strncmp(tmp[i], "<\0", 2))
			i++;
		else
			ret++;
		if (tmp[i])
			i++;
		else
			return (ret);
	}
	return (ret);
}
