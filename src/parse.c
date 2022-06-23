/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:10:12 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/22 18:53:19 by gimartin         ###   ########.fr       */
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
	while (line[meas[0]] != '=' && ft_strncmp(&line[meas[0]], "+=", 2))
		meas[0]++;
	if (line[meas[0]] == '=')
		meas[0]++;
	else
		meas[0] += 2;
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

int	split_command(char *line, t_command *cmd, t_terminfo *terminfo)
{
	char	**tmp;

	if (ft_strchr(line, '|') != NULL)
		check_pipe(line, cmd);
	if (is_var_def(line))
		split_var_decl(line, cmd);
	else
	{
		tmp = split_fields(line, ' ');
		cmd->first = 1;
		if (fill_cmd_fields(tmp, cmd, 0, terminfo) == -1)
			return (0);
	}
	return (1);
}

int	fill_cmd_fields(char **tmp, t_command *cmd, int start, t_terminfo *terminfo)
{
	int		c[3];
	char	**cleaned;
	char	**original;

	original = tmp;
	cleaned = clean_command(tmp, cmd, start, terminfo);
	if (!cleaned)
		return (-1);
	tmp = cleaned;
	if (!tmp[0])
	{
		free(tmp);
		restore_fd(cmd, terminfo);
		return (-1);
	}
	c[2] = count_args(tmp);
	malloc_c_ptr(&cmd->args, c[2] + 1);
	c[0] = start;
	c[1] = 0;
	cpy_and_slide(tmp, c, start, cmd);
	c[0] += start;
	filling_chain(original, cmd, c, terminfo);
	free_array_of_array(tmp);
	return (0);
}
