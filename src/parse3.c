/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 15:49:35 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/13 16:08:21 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**sup_clean_cmd(char **tmp, int start, t_command *cmd, int *j)
{
	char	**cleaned;

	cleaned = malloc(sizeof(char *) * count_cleaned_cmd(&tmp[start]) + 1);
	while (tmp[start] && ft_strncmp(tmp[start], "|\0", 2))
	{
		if (is_redir(tmp[start]) >= 0)
		{
			if (check_redirection(&tmp[start], cmd) != -1)
				start++;
			else
			{
				cleaned[*j] = NULL;
				free_array_of_array(cleaned);
				return (NULL);
			}
		}
		else
		{
			malloc_c(&cleaned[*j], ft_strlen(tmp[start]) + 1);
			ft_strlcpy(cleaned[(*j)++], tmp[start], ft_strlen(tmp[start]) + 1);
		}
		start++;
	}
	return (cleaned);
}

char	**clean_command(char **tmp, t_command *cmd, int start)
{
	int		j;
	char	**cleaned;

	j = 0;
	cleaned = sup_clean_cmd(tmp, start, cmd, &j);
	if (cleaned == NULL)
		return (NULL);
	if (cleaned[0])
		cleaned[j] = NULL;
	else
	{
		free(cleaned);
		restore_fd(cmd);
		return (NULL);
	}
	return (cleaned);
}

void	sup_fill_cmd(char **original, t_command *cmd, int *c)
{
	if (original[c[0]])
	{
		fill_prev(cmd, c, original);
		fill_next(cmd, c, original);
	}
	if (cmd->first)
		free_array_of_array(original);
	else
		cmd->to_pipe_to = 1;
	check_wildcards(cmd);
}

int	fill_cmd_fields(char **tmp, t_command *cmd, int start)
{
	int		c[3];
	char	**cleaned;
	char	**original;

	original = tmp;
	cleaned = clean_command(tmp, cmd, start);
	if (!cleaned)
		return (-1);
	tmp = cleaned;
	if (!tmp[0])
	{
		free(tmp);
		restore_fd(cmd);
		return (-1);
	}
	c[2] = count_args(tmp);
	malloc_c_ptr(&cmd->args, c[2] + 1);
	c[0] = start;
	c[1] = 0;
	cpy_and_slide(tmp, c, start, cmd);
	c[0] += start;
	sup_fill_cmd(original, cmd, c);
	return (0);
}

int	split_command(char *line, t_command *cmd)
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
		if (fill_cmd_fields(tmp, cmd, 0) == -1)
			return (0);
	}
	return (1);
}
