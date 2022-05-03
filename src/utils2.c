/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 14:11:38 by gimartin          #+#    #+#             */
/*   Updated: 2022/04/28 14:21:17 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_text(char *dst, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	malloc_c(&dst, ft_strlen(src));
	while (src[i])
	{
		if (src[i] != '\"')
			dst[j++] = src[i++];
		else
			i++;
	}
	dst[j] = '\0';
}

int	dollar_pos(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '$')
		i++;
	return (i);
}

void	sup_try_expand(t_input *input, int i, int d_quot)
{
	char	*tmp;

	tmp = NULL;
	if (ft_isalpha(input->line[i]) || input->line[i] == '?')
	{
		if (d_quot)
		{
			take_variable(&input->line[i], input, i - 1);
			malloc_c(&tmp, ft_strlen(input->line) + 1);
			ft_strlcpy(tmp, input->line, ft_strlen(input->line) + 1);
			free(input->line);
			clean_text(input->line, tmp);
			free(tmp);
		}
		else
			take_variable(&input->line[i], input, i - 1);
	}
}

void	try_expand(t_input *input)
{
	int		i;
	int		s_quot;
	int		d_quot;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (input->line[i])
	{
		if (input->line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (input->line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		if (input->line[i] == '"' && !s_quot && !d_quot)
			d_quot = 1;
		else if (input->line[i] == '"' && !s_quot && d_quot)
			d_quot = 0;
		if (input->line[i] == '$' && !s_quot)
		{
			i++;
			sup_try_expand(input, i, d_quot);
			return ;
		}
		i++;
	}
}

char	*get_path(char *line)
{
	char	*ret;
	int		i;

	i = 0;
	ret = NULL;
	while (line[i] == 32 || (line[i] > 8 && line[i] < 14))
		i++;
	malloc_c(&ret, ft_strlen(line) - i + 1);
	ft_strlcpy(ret, &line[i], ft_strlen(line) - i + 1);
	return (ret);
}
