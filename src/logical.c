/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 14:29:19 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 14:36:28 by gimartin         ###   ########.fr       */
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

void	start_thinking(char **u_lines, char **rules, t_command *cmd, int count)
{
	int			i;
	char		*clean;
	t_command	tmp;

	i = 0;
	while (i < count)
	{
		if (u_lines[i][0] == '(')
		{
			init_cmd(&tmp);
			clean = single_trim(u_lines[i]);
			get_logical(clean, &tmp);
			free(clean);
		}
		else if (split_command(u_lines[i], cmd))
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
	char	**u_lines;

	i = 0;
	j = 0;
	malloc_c_ptr(&u_lines, count_units(line) + 1);
	while (i < count_units(line))
	{
		malloc_c(&u_lines[i], unit_len(&line[j]) + 1);
		ft_strlcpy(u_lines[i], &line[j], unit_len(&line[j]) + 1);
		i++;
		j += next_unit(&line[j]);
	}
	u_lines[i] = NULL;
	rules = get_rules(line);
	start_thinking(u_lines, rules, cmd, count_units(line));
	free_array_of_array(u_lines);
	free_array_of_array(rules);
}
