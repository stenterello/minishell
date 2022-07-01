/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 16:33:49 by ddelladi          #+#    #+#             */
/*   Updated: 2022/07/01 15:28:18 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_heredocs(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (line[i] && !is_token_char(line[i]))
	{
		if (!ft_strncmp(&line[i], "<<", 2) && !is_open(line, i))
			ret++;
		i++;
	}
	return (ret);
}

void	get_heredoc_input(t_command *cmd, t_terminfo *terminfo)
{
	char	*d;
	char	*inp_line;
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	len = count_heredocs(terminfo->input->line);
	inp_line = NULL;
	while (i++ < len)
	{
		if (cmd->input_line)
		{
			free(cmd->input_line);
			cmd->input_line = NULL;
		}
		d = find_delimiter(cmd->redi, &j);
		cmd->to_exp = to_exp(terminfo->input->line);
		take_heredoc_input(inp_line, d, cmd, terminfo);
	}
}

char	**clean_command(char **tmp, t_command *cmd, int start)
{
	int		j[2];
	char	**cleaned;

	j[0] = start;
	j[1] = 0;
	cleaned = cleaning_loop(tmp, j, cmd);
	if (cleaned == NULL)
		return (NULL);
	if (cleaned[0])
		cleaned[j[1]] = NULL;
	else
	{
		free(cleaned);
		restore_all(cmd);
		return (NULL);
	}
	return (cleaned);
}

void	filling_chain(char **original, t_command *cmd,
	int *c, t_terminfo *terminfo)
{
	if (original[c[0]])
	{
		fill_prev(cmd, c, original);
		fill_next(cmd, c, original, terminfo);
	}
	if (cmd->first)
		free_array_of_array(original);
	else
		cmd->to_pipe_to = 1;
	check_wildcards(cmd);
}

int	treat_heredoc(t_command *tmp, t_terminfo *terminfo)
{
	tmp->delimiter = 1;
	add_signals(terminfo, tmp);
	get_heredoc_input(tmp, terminfo);
	if (heredoc_to_avoid(tmp->args))
		tmp->delimiter = 0;
	else
		tmp->delimiter = 1;
	if (g_child == -1)
	{
		terminfo->last_exit = 130;
		return (1);
	}
	return (0);
}
