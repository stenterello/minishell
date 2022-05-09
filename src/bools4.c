/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bools4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 14:59:13 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 15:13:34 by gimartin         ###   ########.fr       */
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

void	sup_sup_check(char *typed, int i, t_input *input, int *open)
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
}

int	sup_check(char *typed, int i, t_input *input, int *open)
{
	while (typed[i])
	{
		sup_sup_check(typed, i, input, open);
		i++;
	}
	return (i);
}
