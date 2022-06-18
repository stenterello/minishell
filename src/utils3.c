/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:46:45 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/15 14:33:30 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup_sup_try(int d_quot, t_terminfo *terminfo, int i, char *var)
{
	char	*tmp;
	char	*tmp2;

	if (d_quot)
	{
		tmp = define_var_name(terminfo->input->line);
		var = ft_getenv(tmp, terminfo);
		malloc_c(&tmp2, ft_strlen(var)
			+ (ft_strlen(terminfo->input->line) - ft_strlen(tmp)) + 1);
		ft_strlcpy(tmp2, terminfo->input->line,
			until_dollar(terminfo->input->line));
		ft_strlcat(tmp2, var, ft_strlen(var) + ft_strlen(tmp2) + 1);
		ft_strlcat(tmp2, &terminfo->input->line
		[until_end_var_name(terminfo->input->line, tmp)],
			ft_strlen(tmp2)
			+ ft_strlen(&terminfo->input->line
			[until_end_var_name(terminfo->input->line, tmp)]) + 1);
		free(tmp);
		free(terminfo->input->line);
		malloc_c(&terminfo->input->line, ft_strlen(tmp2) + 1);
		ft_strlcpy(terminfo->input->line, tmp2, ft_strlen(tmp2) + 1);
		free(tmp2);
	}
	else
		take_variable(&terminfo->input->line[i], terminfo, i - 1);
}

void	sup_try_expand(t_terminfo *terminfo, int i, int d_quot)
{
	char		*var;

	var = NULL;
	if (terminfo->input->line[i] == '$')
	{
		free(terminfo->input->line);
		malloc_c(&terminfo->input->line, 24);
		ft_strlcpy(terminfo->input->line, "echo can\'t use getpid()", 24);
	}
	else if (ft_isalnum(terminfo->input->line[i])
		|| terminfo->input->line[i] == '?')
		sup_sup_try(d_quot, terminfo, i, var);
}

void	try_expand(t_terminfo *terminfo)
{
	int		i;
	int		s_quot;
	int		d_quot;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (terminfo->input->line[i])
	{
		if (terminfo->input->line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (terminfo->input->line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		if (terminfo->input->line[i] == '"' && !s_quot && !d_quot)
			d_quot = 1;
		else if (terminfo->input->line[i] == '"' && !s_quot && d_quot)
			d_quot = 0;
		if (terminfo->input->line[i] == '$' && !s_quot)
		{
			i++;
			sup_try_expand(terminfo, i, d_quot);
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
