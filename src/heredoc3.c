/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 16:35:00 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/09 12:26:49 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_here(char *delimiter, int i)
{
	ft_putstr_fd(last_field(ft_getenv("SHELL")), 2);
	ft_putstr_fd(": attention: here-document on line ", 2);
	ft_putnbr_fd(i, 2);
	ft_putstr_fd(" is delimited by an EOF (\"", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("\" was required)", 2);
}

void	sup_treat(t_command *cmd, t_command *cmd2, char *typed)
{
	char	*delimiter;
	char	*tmp;

	init_cmd(cmd);
	init_cmd(cmd2);
	cmd->first = 1;
	malloc_c(&cmd2->cmd, ft_strlen(typed) + 1);
	ft_strlcpy(cmd2->cmd, typed, ft_strlen(typed) + 1);
	cmd2->input_line = NULL;
	cmd->input_line = NULL;
	delimiter = take_delimiter(cmd2->cmd);
	clean_heredoc(cmd2->cmd, "<<");
	malloc_c_ptr(&cmd2->args, 2);
	malloc_c(&cmd2->args[0], ft_strlen(cmd2->cmd) + 1);
	ft_strlcpy(cmd2->args[0], cmd2->cmd, ft_strlen(cmd2->cmd) + 1);
	cmd2->args[1] = NULL;
	tmp = readline("> ");
	sup1(delimiter, cmd, tmp, cmd2);
}

int	treat_heredoc(char *typed)
{
	t_command	*cmd;
	t_command	*cmd2;

	g_term.delimiter = 1;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		die("Malloc error");
	cmd2 = malloc(sizeof(t_command));
	if (!cmd2)
		die("Malloc error");
	sup_treat(cmd, cmd2, typed);
	return (1);
}
