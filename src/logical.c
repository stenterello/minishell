/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 14:29:19 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:57:23 by ddelladi         ###   ########.fr       */
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

void	start_thinking(char **units_lines, char **rules, t_command *cmd, int count)
{
	int			i;
	char		*clean;
	t_command	tmp;

	i = 0;
	while (i < count)
	{
		if (units_lines[i][0] == '(')
		{
			init_cmd(&tmp);
			clean = single_trim(units_lines[i]);
			get_logical(clean, &tmp);
			free(clean);
		}
		else if (split_command(units_lines[i], cmd))
			execute_tree(cmd);
		if (i < count - 1)
		{
			if (!ft_strncmp(rules[i], "&&", 2) && g_term.last_exit != 0)
				return ;
			else if (!ft_strncmp(rules[i], "||", 2) && g_term.last_exit == 0)
				return ;
		}
		i++;
	}
}

char	**get_rules(char *line)
{
	int		i;
	int		j;
	char	**ret;

	i = 0;
	j = 0;
	malloc_c_ptr(&ret, count_units(line));
	while (line[i])
	{
		if (!ft_strncmp(&line[i], "&&", 2) && !is_in_par(line, i))
		{
			malloc_c(&ret[j], 3);
			ft_strlcpy(ret[j], "&&\0", 3);
			j++;
		}
		else if (!ft_strncmp(&line[i], "||", 2) && !is_in_par(line, i))
		{
			malloc_c(&ret[j], 3);
			ft_strlcpy(ret[j], "||\0", 3);
			j++;
		}
		i++;
	}
	ret[j] = NULL;
	return (ret);
}

void	get_logical(char *line, t_command *cmd)
{
	int		i;
	int		j;
	char	**rules;
	char	**units_lines;

	i = 0;
	j = 0;
	malloc_c_ptr(&units_lines, count_units(line) + 1);
	while (i < count_units(line))
	{
		malloc_c(&units_lines[i], unit_len(&line[j]) + 1);
		ft_strlcpy(units_lines[i], &line[j], unit_len(&line[j]) + 1);
		i++;
		j += next_unit(&line[j]);
	}
	units_lines[i] = NULL;
	rules = get_rules(line);
	start_thinking(units_lines, rules, cmd, count_units(line));
	free_array_of_array(units_lines);
	free_array_of_array(rules);
}
