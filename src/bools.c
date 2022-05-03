/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:04:49 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:56:12 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sup_is_var_def(char *line, int i, int s_quot, int d_quot)
{
	while (line[i])
	{
		if (line[i] == '\'' && !d_quot && !s_quot)
			s_quot = 1;
		else if (line[i] == '\'' && !d_quot && !s_quot)
			s_quot = 0;
		if (line[i] == '"' && !s_quot && !d_quot)
			d_quot = 1;
		else if (line[i] == '"' && !s_quot && d_quot)
			d_quot = 0;
		if (line[i] == ' ' && !d_quot && !s_quot)
			return (0);
		else if (line[i] == '=' && !d_quot && !s_quot)
			return (1);
		else if (!ft_isascii(line[i]))
			return (0);
		i++;
	}
	return (0);
}

int	is_var_def(char *line)
{
	int	i;
	int	s_quot;
	int	d_quot;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	if (line[0] == '=')
		return (0);
	return (sup_is_var_def(line, i, s_quot, d_quot));
}

int	sup_check(char *typed, int i, t_input *input, int *open)
{
	while (typed[i])
	{
		if (typed[i] == '\'' && !input->s_quot && !input->d_quot)
			input->s_quot = 1;
		else if (typed[i] == '\'' && input->s_quot && !input->d_quot)
			input->s_quot = 0;
		else if (typed[i] == '$' && !input->s_quot)
			input->to_expand = 1;
		else if (typed[i] == '\"' && !input->d_quot && !input->s_quot)
			input->d_quot = 1;
		else if (typed[i] == '\"' && input->d_quot && !input->s_quot)
			input->d_quot = 0;
		else if (typed[i] == '|' && !input->d_quot && !input->s_quot)
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
		i++;
	}
	return (i);
}

int	check_error(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2)) && line[i + 2] && is_logical_token(line[i + 2]))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token \"", 2);
			ft_putchar_fd(line[i + 2], 2);
			ft_putendl_fd("\"", 2);
			return (1);
		}
		else if (is_token(&line[i]))
		{
			while (line[i] == ' ')
				i++;
			if (is_token(&line[i]))
				return (syntax_error_no_arr());
		}
		i++;
	}
	return (0);
}

void	check(char *typed, t_input *input)
{
	int	i;
	int	open[4];

	i = 0;
	input->s_quot = 0;
	input->d_quot = 0;
	input->to_expand = 0;
	open[0] = 0;
	open[1] = 0;
	open[2] = 0;
	open[3] = 0;
	if (!typed)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit(0);
	}
	if (check_error(typed))
	{
		input->with_error = 1;
		return ;
	}
	i = sup_check(typed, i, input, open);
	i--;
	if (typed[i] == '\\' || input->s_quot || input->d_quot
		|| open[0] || open[1])
		input->is_open = 1;
	else if (open[2] != open[3])
		input->is_open = 1;
	else
		input->is_open = 0;
}

int	is_open(char *typed, int limit)
{
	int	i;
	int	s_quot;
	int	d_quot;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (typed[i] && i <= limit)
	{
		if (typed[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (typed[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		else if (typed[i] == '\"' && !d_quot && !s_quot)
			d_quot = 1;
		else if (typed[i] == '\"' && d_quot && !s_quot)
			d_quot = 0;
		i++;
	}
	i--;
	if (s_quot || d_quot)
		return (1);
	return (0);
}
