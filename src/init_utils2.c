/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 18:50:11 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/24 19:05:05 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_matching_opt(char **opt)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		malloc_c(&opt[i], 3);
		if (i == 0)
			ft_strlcpy(opt[i], "&&", 3);
		else if (i == 1)
			ft_strlcpy(opt[i], "||", 3);
		else if (i == 2)
			ft_strlcpy(opt[i], "(", 2);
		else if (i == 3)
			ft_strlcpy(opt[i], ")", 2);
		else if (i == 4)
			ft_strlcpy(opt[i], "\"", 2);
		else if (i == 5)
			ft_strlcpy(opt[i], "\'", 2);
		i++;
	}
	opt[i] = NULL;
}

int	sup_is_match(char **opt, int i, char *line)
{
	if (ft_strlen(opt[i]) == 1)
	{
		if (!ft_strncmp(opt[i], line, 1))
		{
			free_array_of_array(opt);
			return (1);
		}
	}
	else if (ft_strlen(opt[i]) == 2)
	{
		if (!ft_strncmp(opt[i], line, 2))
		{
			free_array_of_array(opt);
			return (2);
		}
	}
	return (0);
}

int	is_matching_token(char *line)
{
	int		i;
	char	**opt;
	int		ret;

	i = 0;
	malloc_c_ptr(&opt, 7);
	copy_matching_opt(opt);
	while (opt[i])
	{
		ret = sup_is_match(opt, i, line);
		if (ret)
			return (ret);
		i++;
	}
	free_array_of_array(opt);
	return (0);
}

char	*find_matching(char *line)
{
	int		i;
	char	*ret;

	i = 0;
	while (line[i])
	{
		if (is_matching_token(&line[i]) == 2)
			return (NULL);
		else if (is_matching_token(&line[i]))
		{
			malloc_c(&ret, 2);
			ft_strlcpy(ret, &line[i], 2);
			return (ret);
		}
		i++;
	}
	return (NULL);
}

void	unexpected(char *typed)
{
	char	*match;

	match = find_matching(typed);
	if (match)
	{
		ft_putstr_fd("bash: unexpected EOF while looking for matching `", 2);
		ft_putstr_fd(match, 2);
		ft_putendl_fd("\'", 2);
	}
	ft_putendl_fd("bash: syntax error: unexpected end of file", 2);
	free_array_of_array(g_term.glob_environ);
	add_history(typed);
	if (match)
		free(match);
	if (typed)
		free(typed);
	main_loop();
}
