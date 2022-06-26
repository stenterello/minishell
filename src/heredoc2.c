/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:16:53 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/26 18:40:32 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	delimiter_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != 32)
		i++;
	return (i);
}

void	false_take_heredoc_input(char *d, t_terminfo *terminfo)
{
	char	*tmp;

	if (terminfo->last_exit == 130)
		terminfo->last_exit = 0;
	tmp = readline("> ");
	while (terminfo->last_exit != 130 && tmp
		&& ft_strncmp(tmp, d, ft_strlen(d)) && g_child != -1)
	{
		free(tmp);
		tmp = readline("> ");
		if (g_child == -1)
			break ;
	}
	free(tmp);
}

char	*take_delimiter(char *line, t_terminfo *terminfo)
{
	int		i;
	char	*ret;
	char	*tmp_delmtr;

	i = 0;
	ret = NULL;
	while (another_heredoc(&line[i]))
	{
		while (line[i] && ft_strncmp(&line[i], "<<", 2))
			i++;
		i = skip_spaces(line, i + 2);
		if (another_heredoc(&line[i]))
		{
			malloc_c(&tmp_delmtr, delimiter_len(&line[i]) + 1);
			ft_strlcpy(tmp_delmtr, &line[i], delimiter_len(&line[i]) + 1);
			false_take_heredoc_input(tmp_delmtr, terminfo);
			free(tmp_delmtr);
		}
	}
	if (line[i] == '\'' || line[i] == '"')
		i++;
	malloc_c(&ret, delimiter_len(&line[i]) + 1);
	ft_strlcpy(ret, &line[i], delimiter_len(&line[i]) + 1);
	return (ret);
}

void	execute_free_here(char *tmp, t_command *cmd2,
	char *d, t_terminfo *terminfo)
{
	t_command	*cmd;

	cmd = (t_command *)cmd2->next;
	if (tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	free(d);
	cmd->next = cmd2;
	cmd2->prev = cmd;
	cmd2->next = NULL;
	define_heredoc_pipe(cmd);
	execute_tree(cmd, terminfo);
	free_single_command(cmd);
	free_single_command(cmd2);
}

void	free_here(char *tmp, char *delimiter, t_command *cmd, t_command *cmd2)
{
	if (tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	free(delimiter);
	free_single_command(cmd);
	if (cmd2->cmd)
		free_single_command(cmd2);
}
