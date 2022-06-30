/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 15:49:35 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/30 17:19:38 by ddelladi         ###   ########.fr       */
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
			|| !ft_strncmp(tmp[i], ">\0", 2)
			|| !ft_strncmp(tmp[i], ">>\0", 3)
			|| !ft_strncmp(tmp[i], "<<\0", 3))
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
		if (is_redir(t[j[0]]) > 0 && is_redir(t[j[0]]) < 3)
			(*c)->redir_out = 1;
		else if (is_redir(t[j[0]]) == 0)
			(*c)->redir_in = 1;
		if (is_redir(t[j[0]]) >= 0)
		{
			malloc_c(&(*c)->redi[i], ft_strlen(t[j[0]]) + 1);
			ft_strlcpy((*c)->redi[i++], t[j[0]], ft_strlen(t[j[0]]) + 1);
			malloc_c(&(*c)->redi[i], ft_strlen(t[++j[0]]) + 1);
			ft_strlcpy((*c)->redi[i++], t[j[0]], ft_strlen(t[j[0]]) + 1);
		}
		else
		{
			malloc_c(&(*cleaned)[j[1]], ft_strlen(t[j[0]]) + 1);
			ft_strlcpy((*cleaned)[j[1]++], t[j[0]], ft_strlen(t[j[0]]) + 1);
		}
		j[0]++;
	}
	if ((*c)->redi)
		(*c)->redi[i] = NULL;
}

char	**cleaning_loop(char **tmp, int j[2],
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

int	is_heredoc2(char **redi)
{
	int	i;

	i = 0;
	if (!redi)
		return (0);
	while (redi[i])
	{
		if (!ft_strncmp(redi[i], "<<\0", 3))
			return (1);
		i++;
	}
	return (0);
}

char	*find_delimiter(char **redi, int *j)
{
	while (redi[*j])
	{
		if (!ft_strncmp(redi[*j], "<<\0", 3))
			return (redi[++(*j)]);
		(*j)++;
	}
	return (NULL);
}
