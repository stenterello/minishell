/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 15:51:58 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/13 10:10:04 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cpy_and_slide(char **tmp, int *c, int start, t_command *cmd)
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

void fill_prev(t_command *cmd, int *c, char **tmp)
{
	t_command *prev;

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

void fill_next(t_command *cmd, int *c, char **tmp)
{
	t_command *next;

	if (tmp[c[0]] && (cmd->to_pipe || cmd->to_pipe_to))
	{
		next = malloc(sizeof(t_command));
		if (!next)
			die("Malloc error");
		init_cmd(next);
		cmd->next = next;
		next->prev = cmd;
		++c[0];
		fill_cmd_fields(tmp, next, c[0]);
	}
}

int there_is_redirection(char **tmp)
{
	int i;

	i = 0;
	while (tmp[i])
	{
		if (!ft_strncmp(tmp[i], ">\0", 2) || !ft_strncmp(tmp[i], ">>\0", 3) || !ft_strncmp(tmp[i], "<\0", 2))
			return (1);
		i++;
	}
	return (0);
}

int count_cleaned_cmd(char **tmp)
{
	int i;
	int ret;

	i = 0;
	ret = 0;
	while (tmp[i] && ft_strncmp(tmp[i], "|\0", 2))
	{
		if (!ft_strncmp(tmp[i], ">\0", 2) || !ft_strncmp(tmp[i], ">>\0", 3) || !ft_strncmp(tmp[i], "<\0", 2))
			i++;
		else
			ret++;
		i++;
	}
	return (ret);
}

char **clean_command(char **tmp, t_command *cmd, int start)
{
	int j;
	char **cleaned;

	j = 0;
	cleaned = malloc(sizeof(char *) * count_cleaned_cmd(&tmp[start]) + 1);
	while (tmp[start] && ft_strncmp(tmp[start], "|\0", 2))
	{
		if (is_redir(tmp[start]) >= 0)
		{
			if (check_redirection(&tmp[start], cmd) != -1)
				start++;
			else
			{
				cleaned[j] = NULL;
				free_array_of_array(cleaned);
				return (NULL);
			}
		}
		else
		{
			malloc_c(&cleaned[j], ft_strlen(tmp[start]) + 1);
			ft_strlcpy(cleaned[j++], tmp[start], ft_strlen(tmp[start]) + 1);
		}
		start++;
	}
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

int fill_cmd_fields(char **tmp, t_command *cmd, int start)
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
	return (0);
}

int split_command(char *line, t_command *cmd)
{
	char **tmp;

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
