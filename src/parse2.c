/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 15:51:58 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/11 17:00:42 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cpy_and_slide(char **tmp, int *c, int start, t_command *cmd)
{
	while (tmp[c[0]] && !is_token(tmp[c[0]]))
	{
		if (c[0] == start)
		{
			malloc_c(&cmd->cmd, next_arg_len(tmp[c[0]]) + 2);
			ft_strlcpy(cmd->cmd, tmp[c[0]], next_arg_len(tmp[c[0]]) + 1);
		}
		if (c[0] != 0 && !ft_strncmp(cmd->cmd, "sed\0", 4))
			tmp[c[0]] = ft_strtrim(tmp[c[0]], "\'");
		malloc_c(&cmd->args[c[1]], ft_strlen(tmp[c[0]]) + 1);
		ft_strlcpy(cmd->args[c[1]], tmp[c[0]], ft_strlen(tmp[c[0]]) + 1);
		c[0]++;
		c[1]++;
	}
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
	while (tmp[i])
	{
		if (!ft_strncmp(tmp[i], ">\0", 2) || !ft_strncmp(tmp[i], ">>\0", 3) || !ft_strncmp(tmp[i], "<\0", 2))
			i++;
		else
			ret++;
		i++;
	}
	return (ret);
}

char **clean_command(char **tmp, t_command *cmd)
{
	int i;
	int j;
	char **cleaned;

	i = 0;
	j = 0;
	cleaned = malloc(sizeof(char *) * count_cleaned_cmd(tmp) + 1);
	while (tmp[i])
	{
		if (is_redir(tmp[i]) >= 0)
		{
			if (check_redirection(&tmp[i], cmd) != -1)
				i++;
			else
			{
				cleaned[j] = NULL;
				free_array_of_array(cleaned);
				return (NULL);
			}
		}
		else
		{
			malloc_c(&cleaned[j], ft_strlen(tmp[i]) + 1);
			ft_strlcpy(cleaned[j++], tmp[i], ft_strlen(tmp[i]) + 1);
		}
		i++;
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
	int c[3];
	char **cleaned;

	cleaned = clean_command(tmp, cmd);
	if (!cleaned)
		return (-1);
	free_array_of_array(tmp);
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
	// if (tmp[c[0]])
	// {
	// 	if (check_redirection(&tmp[c[0]], cmd) == -1)
	// 	{
	// 		syntax_error(tmp);
	// 		free_array_of_array(tmp);
	// 		return (-1);
	// 	}
	// }
	if (tmp[c[0]])
	{
		fill_prev(cmd, c, tmp);
		fill_next(cmd, c, tmp);
	}
	if (cmd->first)
		free_array_of_array(tmp);
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
