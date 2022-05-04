/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 14:11:38 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/04 12:22:05 by ddelladi         ###   ########.fr       */
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

int	until_dollar(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '$')
		i++;
	return (++i);
}

int	until_end_var_name(char *line, char *var)
{
	return (until_dollar(line) + ft_strlen(var));
}

char	*define_var_name(char *line)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	while (line[i] != '$')
		i++;
	j = ++i;
	while (ft_isalnum(line[i]) || line[i] == '?')
		i++;
	malloc_c(&ret, i - j + 2);
	ft_strlcpy(ret, &line[j], i - j + 1);
	return (ret);
}

void	sup_try_expand(t_input *input, int i, int d_quot)
{
	char	*tmp;
	char	*tmp2;
	char	*var;

	tmp = NULL;
	if (ft_isalpha(input->line[i]) || input->line[i] == '?')
	{
		if (d_quot)
		{
			tmp = define_var_name(input->line);
			var = ft_getenv(tmp);
			malloc_c(&tmp2, ft_strlen(var) + (ft_strlen(input->line) - ft_strlen(tmp)) + 1);
			ft_strlcpy(tmp2, input->line, until_dollar(input->line));
			ft_strlcat(tmp2, var, ft_strlen(var) + ft_strlen(tmp2) + 1);
			ft_strlcat(tmp2, &input->line[until_end_var_name(input->line, tmp)],
				ft_strlen(tmp2) +
				ft_strlen(&input->line[until_end_var_name(input->line, tmp)]) + 1);
			free(tmp);
			free(input->line);
			malloc_c(&input->line, ft_strlen(tmp2) + 1);
			ft_strlcpy(input->line, tmp2, ft_strlen(tmp2) + 1);
			free(tmp2);
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
