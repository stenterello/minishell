/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 14:59:13 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/17 15:00:24 by ddelladi         ###   ########.fr       */
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

int	env_exists(char *line, t_terminfo *terminfo)
{
	int	i;

	i = 0;
	if (line == NULL)
		return (0);
	if (terminfo->env)
	{
		while (terminfo->env->key[i])
		{
			if (!ft_strncmp(line, terminfo->env[i].key, ft_strlen(line) + 1))
				return (1);
			i++;
		}
	}
	i = 0;
	if (terminfo->var)
	{
		while (terminfo->var->key[i])
		{
			if (!ft_strncmp(line, terminfo->var[i].key, ft_strlen(line) + 1))
				return (1);
			i++;
		}
	}
	return (0);
}

char	*get_var_name(char *line)
{
	char	*ret;
	int		i;
	int		len;

	ret = NULL;
	i = 0;
	while (ft_isalnum(line[i]))
		i++;
	if (i == 0)
		return (NULL);
	len = i;
	i = -1;
	while (++i < len)
		ret[i] = line[i];
	ret[i] = '\0';
	return (ret);
}

void	sup_sup_check(char *typed, int i, t_terminfo *terminfo, int *open)
{
	if (typed[i] == '\'' && !terminfo->input->s_quot && !terminfo->input->d_quot)
		terminfo->input->s_quot = 1;
	else if (typed[i] == '\'' && terminfo->input->s_quot && !terminfo->input->d_quot)
		terminfo->input->s_quot = 0;
	else if (typed[i] == '$' && !terminfo->input->s_quot && typed[i + 1] && (typed[i + 1] == '?' || env_exists(get_var_name(&typed[i + 1]), terminfo)))
		terminfo->input->to_expand = 1;
	else if (typed[i] == '\"' && !terminfo->input->d_quot && !terminfo->input->s_quot)
		terminfo->input->d_quot = 1;
	else if (typed[i] == '\"' && terminfo->input->d_quot && !terminfo->input->s_quot)
		terminfo->input->d_quot = 0;
	else if (typed[i] == '|' && !terminfo->input->d_quot && !terminfo->input->s_quot)
		open[0] = 1;
	else if (ft_isalnum(typed[i]) && open[0])
		open[0] = 0;
	else if (!open[1] && (!ft_strncmp(&typed[i], "&&", 2)
			|| !ft_strncmp(&typed[i], "||", 2)))
		open[1] = 1;
	else if (typed[i] == '(' && !is_open(typed, i))
		open[2]++;
	else if (typed[i] == ')' && !is_open(typed, i))
		open[3]++;
	if (ft_isalnum(typed[i]) && open[1])
		open[1] = 0;
}

int	sup_check(char *typed, int i, t_terminfo *terminfo, int *open)
{
	while (typed[i])
	{
		sup_sup_check(typed, i, terminfo, open);
		i++;
	}
	return (i);
}
