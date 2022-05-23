/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 14:29:19 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/23 15:20:56 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*single_trim(char const *s1)
{
	char	*newstr;

	if (!s1)
		return (NULL);
	newstr = malloc(sizeof(char) * (ft_strlen(s1) - 1));
	if (!newstr)
		return (NULL);
	ft_strlcpy(newstr, &s1[1], ft_strlen(s1) - 1);
	return (newstr);
}

int	unit_len2(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (line[i] == ' ')
		i++;
	if ((line[i] == '(' || line[i] == ')') && !is_open(line, i))
		return (1);
	else if ((!ft_strncmp("&&", &line[i], 2) || !ft_strncmp("||", &line[i], 2)) && !is_open(line, i))
		return (2);
	else
	{
		while ((ft_strncmp("&&", &line[i], 2) && ft_strncmp("||", &line[i], 2) && line[i] != '(' && line[i] != ')' && !is_open(line, i)))
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

int	next_unit2(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (line[i] && line[i] == ' ')
		i++;
	if (line[i] == '(' || line[i] == ')')
	{
		i++;
		ret++;
		while (line[i] == ' ')
		{
			i++;
			ret++;
		}
		return (ret);
	}
	else if ((!ft_strncmp(&line[i], "&&", 2) || !ft_strncmp(&line[i], "||", 2)) && !is_open(line, i))
	{
		ret = 2;
		i += ret;
		while (line[i] && line[i] == ' ')
		{
			i++;
			ret++;
		}
		return (ret);
	}
	else
	{
		while ((ft_strncmp("&&", &line[i], 2) && ft_strncmp("||", &line[i], 2) && line[i] != '(' && line[i] != ')' && !is_open(line, i)))
		{
			i++;
			ret++;
		}
		return (ret);
	}
}

int	n_lvls(char **l)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (l[i])
	{
		if (!ft_strncmp("(", l[i], 1))
			ret++;
		i++;
	}
	return (ret);
}

int	next_logic_unit(char **l, int *lvl)
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

void	start_thinking(char **u_lines, t_command *cmd)
{
	int	*exits;
	int	level;
	int	i;

	exits = malloc(sizeof(int) * n_lvls(u_lines) + 1);
	i = 0;
	while (i < n_lvls(u_lines))
		exits[i++] = 0;
	level = 0;
	i = 0;
	while (u_lines[i])
	{
		if (ft_isalnum(u_lines[i][0]))
		{
			if (split_command(u_lines[i], cmd))
				execute_tree(cmd);
			exits[level] = g_term.last_exit;
		}
		if (u_lines[i][0] == '(')
			level++;
		else if (u_lines[i][0] == ')')
			level--;
		else if (!ft_strncmp("&&", u_lines[i], 2) && exits[level] != 0)
			i += next_logic_unit(&u_lines[i], &level);
		else if (!ft_strncmp("||", u_lines[i], 2) && exits[level] == 0)
			i += next_logic_unit(&u_lines[i], &level);
		if (u_lines[i])
			i++;
	}
	free(exits);
}

void	get_logical(char *line, t_command *cmd)
{
	int		i;
	int		j;
	char	**u_lines;

	i = 0;
	j = 0;
	malloc_c_ptr(&u_lines, count_units(line) + 1);
	while (i < count_units(line))
	{
		malloc_c(&u_lines[i], unit_len2(&line[j]) + 1);
		ft_strlcpy(u_lines[i], &line[j], unit_len2(&line[j]) + 1);
		i++;
		j += next_unit2(&line[j]);
	}
	u_lines[i] = NULL;
	start_thinking(u_lines, cmd);
	free_array_of_array(u_lines);
}
