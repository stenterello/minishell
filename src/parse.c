/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:10:12 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/07 12:26:43 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_arg(char *line, t_command *cmd, int meas[3])
{
	malloc_c(&cmd->args[meas[2]], (meas[0] - meas[1] + 1));
	ft_strlcpy(cmd->args[meas[2]], &line[meas[1]], meas[0] - meas[1] + 1);
	cmd->args[meas[2] + 1] = NULL;
}

void	sup_split_var(char *line, int *meas, t_command *cmd)
{
	while (line[meas[0]])
	{
		if (line[meas[0]] == ' ' && !is_open(line, meas[0]))
		{
			fill_arg(line, cmd, meas);
			meas[2]++;
			while (line[meas[0]] == ' ')
				meas[0]++;
			meas[1] = meas[0];
		}
		meas[0]++;
	}
}

void	split_var_decl(char *line, t_command *cmd)
{
	int	meas[3];

	meas[0] = 0;
	meas[1] = 0;
	meas[2] = 0;
	malloc_c_ptr(&cmd->args, count_params(line) + 1);
	cmd->args[0] = NULL;
	while (line[meas[0]] != '=')
		meas[0]++;
	meas[0]++;
	sup_split_var(line, meas, cmd);
	if (!cmd->args[meas[2]])
	{
		malloc_c(&cmd->args[meas[2]], ft_strlen(line) + 1);
		ft_strlcpy(cmd->args[meas[2]], &line[meas[1]],
			ft_strlen(&line[meas[1]]) + 1);
		meas[2]++;
	}
	cmd->args[meas[2]] = NULL;
}

void	free_array_of_array(char **arr)
{
	int	i;

	i = 0;
	if (arr[i])
	{
		while (arr[i])
			free(arr[i++]);
	}
	if (arr)
		free(arr);
}

void	syntax_error(char **tmp)
{
	int		i;
	char	tok[8];

	ft_putstr_fd(last_field(ft_getenv("SHELL")), 2);
	ft_putstr_fd(": syntax error near unexpected token \"", 2);
	i = 0;
	while (tmp[i])
	{
		if (is_token(tmp[i]))
			ft_strlcpy(tok, tmp[i], ft_strlen(tmp[i]) + 1);
		i++;
	}
	tok[ft_strlen(tmp[i])] = '\0';
	if (tok[0] == '<' || tok[0] == '>')
		ft_strlcpy(tok, "newline", 8);
	ft_putstr_fd(tok, 2);
	ft_putendl_fd("\"", 2);
}
