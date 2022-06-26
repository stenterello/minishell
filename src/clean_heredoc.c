/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/26 18:22:42 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/26 18:49:03 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_heredoc_args(char *line)
{
	int	ret;
	int	i;

	i = 0;
	ret = 0;
	while (line[i])
	{
		if (!ft_strncmp(&line[i], "<<", 2))
			i = skip_spaces(line, i + 2)
				+ delimiter_len(&line[skip_spaces(line, i + 2)]);
		else if (((line[i] > 8 && line[i] < 14) || line[i] == 32)
			&& ft_strncmp(&line[i + 1], "<<", 2))
		{
			i = skip_spaces(line, i);
			if (line[i])
			{
				ret++;
				i += delimiter_len(&line[i]);
			}
		}
		else
			i++;
	}
	return (ret);
}

static int	count_heredoc_spaces(char *line)
{
	int	ret;
	int	i;

	i = 0;
	ret = 0;
	while (line[i])
	{
		if ((line[i] > 8 && line[i] < 14) || line[i] == 32)
			ret++;
		i++;
	}
	return (ret);
}

static int	find_delimiter_len(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (another_heredoc(&line[i]))
	{
		while (line[i] && ft_strncmp(&line[i], "<<", 2))
			i++;
		i = skip_spaces(line, i + 2);
		ret += delimiter_len(&line[i]) + 2;
	}
	return (ret);
}

static void	take_heredoc_args(int *i, int *j, t_command *cmd)
{
	while (cmd->cmd[*i])
	{
		*i = skip_spaces(cmd->cmd, *i);
		if (cmd->cmd[*i] && ft_strncmp(&cmd->cmd[*i], "<<", 2))
		{
			malloc_c(&cmd->args[*j], delimiter_len(&cmd->cmd[*i]) + 1);
			ft_strlcpy(cmd->args[(*j)++], &cmd->cmd[*i],
				delimiter_len(&cmd->cmd[*i]) + 1);
			*i += delimiter_len(&cmd->cmd[*i]);
		}
		else if (cmd->cmd[*i] && !ft_strncmp(&cmd->cmd[*i], "<<", 2))
			*i = skip_spaces(cmd->cmd, *i + 2)
				+ delimiter_len(&cmd->cmd[skip_spaces(cmd->cmd, *i + 2)]);
		else if (!cmd->cmd[*i])
			break ;
		else
			(*i)++;
	}
}

void	clean_heredoc(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	if (count_heredoc_args(cmd->cmd))
		malloc_c_ptr(&cmd->args, count_heredoc_args(cmd->cmd) + 2);
	i += delimiter_len(cmd->cmd);
	take_heredoc_args(&i, &j, cmd);
	if (count_heredoc_args(cmd->cmd))
		cmd->args[j] = NULL;
	j = 1;
	i -= (count_heredoc_spaces(cmd->cmd) + find_delimiter_len(cmd->cmd));
	while (cmd->args && cmd->args[j])
		i -= ft_strlen(cmd->args[j++]);
	while (cmd->cmd[i] == 32)
		i--;
	cmd->cmd[++i] = '\0';
}
