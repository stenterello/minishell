/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:16:53 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/25 10:18:46 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	delimiter_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] < 8 || line[i] > 14) && line[i] != 32)
		i++;
	return (i);
}

char	*take_delimiter(char *line)
{
	int		i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (line[i] && ft_strncmp(&line[i], "<<", 2))
		i++;
	i += 2;
	while (line[i] && ((line[i] > 8 && line[i] < 14) || line[i] == 32))
		i++;
	if (line[i] == '\'' || line[i] == '"')
		i++;
	malloc_c(&ret, delimiter_len(&line[i]) + 1);
	ft_strlcpy(ret, &line[i], delimiter_len(&line[i]) + 1);
	return (ret);
}

void	clean_heredoc(char *line, char *bench)
{
	int	i;

	i = 0;
	while (ft_strncmp(&line[i], bench, ft_strlen(bench))
		&& i <= (int)(ft_strlen(line) - (int)ft_strlen(bench)))
		i++;
	while (!ft_isalnum(line[i]))
		i--;
	line[++i] = '\0';
}

void	execute_free_here(char *tmp, t_command *cmd,
	t_command *cmd2, t_terminfo *terminfo)
{
	if (tmp)
	{
		free(tmp);
		tmp = NULL;
	}
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
	free_single_command(cmd2);
}
