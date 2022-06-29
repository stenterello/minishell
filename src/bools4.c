/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 14:59:13 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/29 10:55:19 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_check2(char *typed, t_input *input, int *open, int i)
{
	if (typed[i] == '\\' || input->s_quot || input->d_quot
		|| open[0] || open[1])
		input->is_open = 1;
	else if (open[2] != open[3])
		input->is_open = 1;
	else
		input->is_open = 0;
}

char	*get_var_name(char *line)
{
	char	*ret;
	int		i;
	int		len;

	ret = NULL;
	if (!line)
		return (NULL);
	i = 0;
	while (ft_isalnum(line[i]))
		i++;
	if (i == 0)
		return (NULL);
	len = i;
	malloc_c(&ret, len + 1);
	ft_strlcpy(ret, line, len + 1);
	return (ret);
}

int	sup_env_ex(t_terminfo *terminfo, t_dict *tmp, char *ret)
{
	tmp = terminfo->var;
	while (tmp)
	{
		if (!ft_strncmp(ret, tmp->key, ft_strlen(ret) + 1))
		{
			free(ret);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	env_exists(char *line, t_terminfo *terminfo)
{
	char	*ret;
	t_dict	*tmp;

	ret = get_var_name(line);
	if (!ret)
		return (0);
	tmp = terminfo->env;
	while (tmp)
	{
		if (!ft_strncmp(ret, tmp->key, ft_strlen(ret) + 1))
		{
			free(ret);
			return (1);
		}
		tmp = tmp->next;
	}
	return (sup_env_ex(terminfo, tmp, ret));
}

int	another_heredoc(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_strncmp(&line[i], "<<", 2) && !is_token(&line[i]))
		i++;
	if (!line[i])
		return (0);
	return (1);
}
