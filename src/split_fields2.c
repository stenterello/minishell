/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_fields2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 17:00:03 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/21 11:51:51 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_count_fields(int *p, char *s, char c)
{
	if (s[p[0]] == c && !is_open(s, p[0]))
		p[2] = 1;
	else if (!ft_strncmp(&s[p[0]], ">>", 2) && !is_open(s, p[0]))
	{
		p[1]++;
		p[0]++;
		p[2] = 1;
	}
	else if ((s[p[0]] == '|' || s[p[0]] == '>' || s[p[0]] == '<')
		&& !is_open(s, p[0]))
	{
		p[1]++;
		p[2] = 1;
	}
	else if (s[p[0]] != c && p[2] == 1 && !is_open(s, p[0]))
	{
		p[1]++;
		p[2] = 0;
		if (is_chaining_token(s[p[0]]) && !is_open(s, p[0]))
			p[2] = 1;
	}
}

int	count_fields(char *s, char c)
{
	int	p[3];

	p[0] = 0;
	p[1] = 0;
	p[2] = 1;
	while (s[p[0]])
	{
		sup_count_fields(p, s, c);
		p[0]++;
	}
	return (p[1]);
}

void	sup_split(char **ret, char *s, int *p, char c)
{
	char	*tmp;

	ft_strlcpy(ret[p[0]], &s[p[1]], count_len(&s[p[1]], c) + 1);
	if (ret[p[0]][0] == '\"')
	{
		tmp = ft_strtrim(ret[p[0]], "\"");
		free(ret[p[0]]);
		malloc_c(&ret[p[0]], ft_strlen(tmp) + 1);
		ft_strlcpy(ret[p[0]], tmp, ft_strlen(tmp) + 1);
		free(tmp);
	}
	else if (ret[p[0]][0] == '\'')
	{
		tmp = ft_strtrim(ret[p[0]], "\'");
		free(ret[p[0]]);
		malloc_c(&ret[p[0]], ft_strlen(tmp) + 1);
		ft_strlcpy(ret[p[0]], tmp, ft_strlen(tmp) + 1);
		free(tmp);
	}
}

char	**split_fields(char *s, char c)
{
	int		p[3];
	char	**ret;

	if (!s)
		return (NULL);
	p[2] = count_fields(s, c);
	malloc_c_ptr(&ret, p[2] + 1);
	p[0] = 0;
	p[1] = find_start(s, c);
	while (p[0] < p[2] && s[p[1]])
	{
		malloc_c(&ret[p[0]], count_len(&s[p[1]], c) + 1);
		sup_split(ret, s, p, c);
		p[1] += find_next_start(&s[p[1]], c);
		p[0]++;
	}
	ret[p[0]] = NULL;
	return (ret);
}
