/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:46:45 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 15:47:05 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_sup_try(int d_quot, t_input *in, int i, char *var)
{
	char	*tmp;
	char	*tmp2;

	tmp = NULL;
	if (d_quot)
	{
		tmp = define_var_name(in->line);
		var = ft_getenv(tmp);
		malloc_c(&tmp2, ft_strlen(var)
			+ (ft_strlen(in->line) - ft_strlen(tmp)) + 1);
		ft_strlcpy(tmp2, in->line, until_dollar(in->line));
		ft_strlcat(tmp2, var, ft_strlen(var) + ft_strlen(tmp2) + 1);
		ft_strlcat(tmp2, &in->line[until_end_var_name(in->line, tmp)],
			ft_strlen(tmp2)
			+ ft_strlen(&in->line[until_end_var_name(in->line, tmp)]) + 1);
		free(tmp);
		free(in->line);
		malloc_c(&in->line, ft_strlen(tmp2) + 1);
		ft_strlcpy(in->line, tmp2, ft_strlen(tmp2) + 1);
		free(tmp2);
	}
	else
		take_variable(&in->line[i], in, i - 1);
}

void	sup_try_expand(t_input *in, int i, int d_quot)
{
	char	*var;

	var = NULL;
	if (ft_isalnum(in->line[i]) || in->line[i] == '?')
		sup_sup_try(d_quot, in, i, var);
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
