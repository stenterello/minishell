/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 15:49:35 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/15 15:13:25 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**cleaning_loop(char **tmp, int j[2],
	t_command *cmd, t_terminfo *terminfo)
{
	char	**cleaned;

	cleaned = malloc(sizeof(char *) * count_cleaned_cmd(&tmp[j[0]]) + 1);
	while (tmp[j[0]] && ft_strncmp(tmp[j[0]], "|\0", 2))
	{
		if (is_redir(tmp[j[0]]) >= 0)
		{
			if (check_redirection(&tmp[j[0]], cmd, terminfo) != -1)
				j[0]++;
			else
			{
				cleaned[j[1]] = NULL;
				free_array_of_array(cleaned);
				return (NULL);
			}
		}
		else
		{
			malloc_c(&cleaned[j[1]], ft_strlen(tmp[j[0]]) + 1);
			ft_strlcpy(cleaned[(j[1])++], tmp[j[0]], ft_strlen(tmp[j[0]]) + 1);
		}
		j[0]++;
	}
	return (cleaned);
}

char	**clean_command(char **tmp, t_command *cmd,
	int start, t_terminfo *terminfo)
{
	int		j[2];
	char	**cleaned;

	j[0] = start;
	j[1] = 0;
	cleaned = cleaning_loop(tmp, j, cmd, terminfo);
	if (cleaned == NULL)
		return (NULL);
	if (cleaned[0])
		cleaned[j[1]] = NULL;
	else
	{
		free(cleaned);
		restore_all(cmd, terminfo);
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

int	fill_cmd_fields(char **tmp, t_command *cmd, int start, t_terminfo *terminfo)
{
	int		c[3];
	char	**cleaned;
	char	**original;

	original = tmp;
	cleaned = clean_command(tmp, cmd, start, terminfo);
	if (!cleaned)
		return (-1);
	tmp = cleaned;
	if (!tmp[0])
	{
		free(tmp);
		restore_fd(cmd, terminfo);
		return (-1);
	}
	c[2] = count_args(tmp);
	malloc_c_ptr(&cmd->args, c[2] + 1);
	c[0] = start;
	c[1] = 0;
	cpy_and_slide(tmp, c, start, cmd);
	c[0] += start;
	filling_chain(original, cmd, c, terminfo);
	free_array_of_array(tmp);
	return (0);
}

int	split_command(char *line, t_command *cmd, t_terminfo *terminfo)
{
	char	**tmp;

	if (ft_strchr(line, '|') != NULL)
		check_pipe(line, cmd);
	if (is_var_def(line))
		split_var_decl(line, cmd);
	else
	{
		tmp = split_fields(line, ' ');
		cmd->first = 1;
		if (fill_cmd_fields(tmp, cmd, 0, terminfo) == -1)
			return (0);
	}
	return (1);
}
