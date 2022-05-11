/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_bools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 11:00:24 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/10 11:24:34 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_islower(char c)
{
	if (97 <= c && c <= 122)
		return (1);
	return (0);
}

int	is_wildcard(char c)
{
	if (c == '*' || c == '?' || c == '[')
		return (1);
	return (0);
}

int	is_in(char *range, char c)
{
	int	i;

	i = 0;
	while (range[i])
	{
		if (c == range[i])
			return (1);
		i++;
	}
	return (0);
}

int	is_verified_brackets(char *brackets, char file_char)
{
	char	*rules;

	if (count_letters(brackets) == -1)
		return (-1);
	rules = get_letters(brackets);
	if (is_in(rules, file_char))
	{
		free(rules);
		return (1);
	}
	free(rules);
	return (0);
}

int	has_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '?' || str[i] == '*' || str[i] == '[')
			&& !is_open(str, i))
			return (1);
		i++;
	}
	return (0);
}
