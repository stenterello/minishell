/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 14:29:19 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/20 13:12:30 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unit_len(char *line)
{
	int	i;
	int	ret;

	i = skip_spaces(line, 0);
	ret = 0;
	if ((line[i] == '(' || line[i] == ')') && !is_open(line, i))
		return (1);
	else if ((!ft_strncmp("&&", &line[i], 2)
			|| !ft_strncmp("||", &line[i], 2)) && !is_open(line, i))
		return (2);
	else
	{
		while (line[i] && (ft_strncmp("&&", &line[i], 2) && ft_strncmp("||", &line[i], 2)
				&& line[i] != '(' && line[i] != ')'))
		{
			i++;
			ret++;
		}
		while (line[i - 1] == ' ')
		{
			i--;
			ret--;
		}
		return (ret);
	}
}

int	next_unit(char *line)
{
	int	i;
	int	ret;

	i = skip_spaces(line, 0);
	ret = 0;
	if (line[i] == '(' || line[i] == ')')
	{
		increment_couple(&i, &ret);
		ret += skip_spaces(line, 0);
		return (ret);
	}
	else if ((!ft_strncmp(&line[i], "&&", 2)
			|| !ft_strncmp(&line[i], "||", 2)) && !is_open(line, i))
	{
		ret = 2;
		ret += skip_spaces(&line[ret], 0);
		return (ret);
	}
	else
	{
		while (line[i] && (ft_strncmp("&&", &line[i], 2) && ft_strncmp("||", &line[i], 2)
				&& line[i] != '(' && line[i] != ')'))
			increment_couple(&i, &ret);
		return (ret);
	}
}

int	next_chained_unit(char **l, int *lvl)
{
	int	i;
	int	n_lvl;

	i = 1;
	n_lvl = *lvl;
	while (l[i])
	{
		if (!ft_strncmp(l[i], "(", 1))
			n_lvl++;
		else if (!ft_strncmp(l[i], ")", 1))
			break ;
		if (!ft_strncmp(l[i], "&&", 2) || !ft_strncmp(l[i], "||", 2))
			return (i);
		i++;
	}
	return (i);
}

void	remove_handcuffs(char **u_lines, t_command *cmd, t_terminfo *terminfo)
{
	int	*exits;
	int	c[2];

	exits = NULL;
	create_exits(&exits, u_lines);
	ft_bzero((void *)c, 8);
	while (u_lines[c[0]])
	{
		if (ft_isalnum(u_lines[c[0]][0])
			&& split_command(u_lines[c[0]], cmd, terminfo))
		{
			execute_tree(cmd, terminfo);
			exits[c[1]] = terminfo->last_exit;
		}
		if (u_lines[c[0]][0] == '(')
			c[1]++;
		else if (u_lines[c[0]][0] == ')')
			c[1]--;
		else if ((!ft_strncmp("&&", u_lines[c[0]], 2) && exits[c[1]] != 0)
			|| (!ft_strncmp("||", u_lines[c[0]], 2) && exits[c[1]] == 0))
			c[0] += next_chained_unit(&u_lines[c[0]], &c[1]);
		if (u_lines[c[0]])
			c[0]++;
	}
	free(exits);
}

void	get_keys(char *line, t_command *cmd, t_terminfo *terminfo)
{
	int		i;
	int		j;
	char	**u_lines;

	i = 0;
	j = 0;
	malloc_c_ptr(&u_lines, count_units(line, 0) + 1);
	while (i < count_units(line, 0))
	{
		malloc_c(&u_lines[i], unit_len(&line[j]) + 1);
		ft_strlcpy(u_lines[i], &line[j], unit_len(&line[j]) + 1);
		i++;
		j += next_unit(&line[j]);
	}
	u_lines[i] = NULL;
	remove_handcuffs(u_lines, cmd, terminfo);
	free_array_of_array(u_lines);
}
