/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 16:12:15 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/10 16:09:47 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_wildcards(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (has_wildcard(cmd->args[i]))
		{
			if (guess(cmd, i) == -1)
				return (-1);
		}
		i++;
	}
	if (ft_strncmp(cmd->args[0], cmd->cmd, ft_strlen(cmd->args[0]) + 1))
	{
		free(cmd->cmd);
		malloc_c(&cmd->cmd, ft_strlen(cmd->args[0]) + 1);
		ft_strlcpy(cmd->cmd, cmd->args[0], ft_strlen(cmd->args[0]) + 1);
	}
	return (0);
}

int	hidden_files(char *str)
{
	if (!ft_strncmp(".\0", str, 2) || !ft_strncmp("..\0", str, 3))
		return (1);
	return (0);
}

int	guess2(t_command *cmd, char **tmp)
{
	if (!hidden_files(cmd->portions[0]))
	{
		if (count_results(cmd->portions) == -1)
			helper_guess(cmd);
		else if (count_results(cmd->portions) == 0)
			return (0);
		tmp = NULL;
		return (free_guess(tmp, cmd));
	}
	else
	{
		if (count_h_results(cmd->portions) == -1)
			helper_guess(cmd);
		else if (count_h_results(cmd->portions) == 0)
			return (0);
		tmp = NULL;
		return (free_hidden_guess(tmp, cmd));
	}
}

int	guess(t_command *cmd, int i)
{
	int		j;
	int		k;
	int		ret;
	char	**tmp;

	j = 0;
	k = 0;
	malloc_c_ptr(&cmd->portions, count_portions(cmd->args[i]) + 1);
	while (cmd->args[i][j])
	{
		ret = sup_guess(cmd, i, j, k++);
		j += ft_strlen(cmd->portions[k - 1]);
	}
	if (ret == -1)
		return (-1);
	cmd->portions[k] = NULL;
	tmp = NULL;
	return (guess2(cmd, tmp));
}
