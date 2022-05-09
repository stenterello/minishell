/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:16:53 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 14:42:59 by gimartin         ###   ########.fr       */
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
	free(tmp);
	free(delimiter);
	cmd->next = cmd2;
	cmd2->prev = cmd;
	define_heredoc_pipe(cmd);
	execute_tree(cmd);
	free_single_command(cmd);
	free_single_command(cmd2);
}

void	sup1(char *delimiter, t_command *cmd, char *tmp, t_command *cmd2)
{
	int	i;

	i = 1;
	while (tmp && ft_strncmp(tmp, delimiter, ft_strlen(delimiter) + 1))
	{
		if (cmd->input_line)
			sup1_sup1(cmd, tmp);
		else
		{
			malloc_c(&cmd->input_line, ft_strlen(tmp) + 1);
			ft_strlcpy(cmd->input_line, tmp, ft_strlen(tmp) + 1);
			ft_strlcat(cmd->input_line, "\n", ft_strlen(cmd->input_line) + 2);
		}
		free(tmp);
		i++;
		tmp = readline("> ");
	}
	if (!tmp)
		print_here(delimiter, i);
	free_here(tmp, delimiter, cmd, cmd2);
}

void	init_here(t_command *cmd, t_command *cmd2)
{
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		die("Malloc error");
	cmd2 = malloc(sizeof(t_command));
	if (!cmd2)
		die("Malloc error");
}
