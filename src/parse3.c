/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 15:49:35 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/24 17:37:15 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_redirections_fields(char **tmp)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (tmp[i] && tmp[i][0] != '|')
	{
		if (!ft_strncmp(tmp[i], "<\0", 2)
			|| ft_strncmp(tmp[i], ">\0", 2)
			|| !ft_strncmp(tmp[i], ">>\0", 3))
			ret += 2;
		i++;
	}
	return (ret);
}

void	sup_cleaning_loop(char **t, t_command **c, int j[2], char ***cleaned)
{
	int	i;

	i = 0;
	while (t[j[0]] && ft_strncmp(t[j[0]], "|\0", 2))
	{
		if (is_redir(t[j[0]]) > 0)
			(*c)->redir_out = 1;
		else if (is_redir(t[j[0]]) == 0)
			(*c)->redir_in = 1;
		if (is_redir(t[j[0]]) >= 0)
		{
			malloc_c(&(*c)->redi[i], ft_strlen(t[j[0]]) + 1);
			ft_strlcpy((*c)->redi[i++], t[j[0]], ft_strlen(t[j[0]]) + 1);
			j[0]++;
			malloc_c(&(*c)->redi[i], ft_strlen(t[j[0]]) + 1);
			ft_strlcpy((*c)->redi[i++], t[j[0]], ft_strlen(t[j[0]]) + 1);
		}
		else
		{
			malloc_c(&(*cleaned)[j[1]], ft_strlen(t[j[0]]) + 1);
			ft_strlcpy((*cleaned)[j[1]++], t[j[0]], ft_strlen(t[j[0]]) + 1);
		}
		j[0]++;
	}
	(*c)->redi[i] = NULL;
}

static char	**cleaning_loop(char **tmp, int j[2],
	t_command *cmd)
{
	char	**cleaned;

	cleaned = NULL;
	malloc_c_ptr(&cleaned, count_cleaned_cmd(&tmp[j[0]]) + 1);
	if (count_redirections_fields(tmp) > 0)
		malloc_c_ptr(&cmd->redi, count_redirections_fields(tmp) + 1);
	sup_cleaning_loop(tmp, &cmd, j, &cleaned);
	cleaned[j[1]] = NULL;
	return (cleaned);
}

char	**clean_command(char **tmp, t_command *cmd,
	int start, t_terminfo *terminfo)
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
