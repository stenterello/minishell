/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:16:53 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/14 14:19:59 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sup1_sup1(t_command *cmd, char *tmp)
{
	char	*tmp2;

	tmp2 = NULL;
	malloc_c(&tmp2, ft_strlen(cmd->input_line) + 1);
	ft_strlcpy(tmp2, cmd->input_line, ft_strlen(cmd->input_line) + 1);
	free(cmd->input_line);
	malloc_c(&cmd->input_line, ft_strlen(tmp2) + ft_strlen(tmp) + 3);
	ft_strlcpy(cmd->input_line, tmp2, ft_strlen(tmp2) + 1);
	ft_strlcat(cmd->input_line, tmp,
		ft_strlen(tmp) + ft_strlen(tmp2) + 2);
	ft_strlcat(cmd->input_line, "\n", ft_strlen(cmd->input_line) + 2);
	free(tmp2);
}

void	free_single_command(t_command *cmd)
{
	int	i;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->input_line)
		free(cmd->input_line);
}

void	free_here(char *tmp, char *delimiter, t_command *cmd, t_command *cmd2)
{
	if (tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	free(delimiter);
	cmd->next = cmd2;
	cmd2->prev = cmd;
	define_heredoc_pipe(cmd);
	execute_tree(cmd);
	free_single_command(cmd);
	free_single_command(cmd2);
}
