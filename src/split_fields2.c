/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_fields2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 17:00:03 by gimartin          #+#    #+#             */
/*   Updated: 2022/04/28 18:20:09 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_count_fields(int *p, char *s, char c)
{
	if (s[p[0]] != c && p[2] == 1 && !is_open(s, p[0]))
	{
		p[1]++;
		p[2] = 0;
	}
	else if (s[p[0]] == c && !is_open(s, p[0]))
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
	ret = malloc(sizeof(char *) * (p[2] + 1));
	if (!ret)
		return (NULL);
	p[0] = 0;
	p[1] = find_start(s, c);
	while (p[0] < p[2])
	{
		ret[p[0]] = malloc(sizeof(char) * count_len(&s[p[1]], c) + 1);
		if (!ret[p[0]])
			return (NULL);
		sup_split(ret, s, p, c);
		p[1] += find_next_start(&s[p[1]], c);
		p[0]++;
	}
	ret[p[0]] = NULL;
	return (ret);
}
