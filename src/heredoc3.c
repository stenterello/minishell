/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 16:35:00 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/10 17:17:01 by ddelladi         ###   ########.fr       */
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

char	*sup_sup_treat(char *tmp, int i, char *d, t_command *cmd)
{
	if (g_term.last_exit == 130)
		g_term.last_exit = 0;
	tmp = readline("> ");
	while (g_term.last_exit != 130 && tmp && ft_strncmp(tmp, d, ft_strlen(d)))
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
		print_here(d, i);
	return (tmp);
}

void	sup_treat(t_command *cmd, t_command *cmd2, char *typed)
{
	char	*d;
	char	*tmp;
	int		i;

	i = 1;
	tmp = NULL;
	init_cmd(cmd);
	init_cmd(cmd2);
	cmd->first = 1;
	malloc_c(&cmd2->cmd, ft_strlen(typed) + 1);
	ft_strlcpy(cmd2->cmd, typed, ft_strlen(typed) + 1);
	cmd2->input_line = NULL;
	cmd->input_line = NULL;
	cmd->next = cmd2;
	d = take_delimiter(cmd2->cmd);
	clean_heredoc(cmd2->cmd, "<<");
	malloc_c_ptr(&cmd2->args, 2);
	malloc_c(&cmd2->args[0], ft_strlen(cmd2->cmd) + 1);
	ft_strlcpy(cmd2->args[0], cmd2->cmd, ft_strlen(cmd2->cmd) + 1);
	cmd2->args[1] = NULL;
	tmp = sup_sup_treat(tmp, i, d, cmd);
	if (g_term.last_exit != 130)
		free_here(tmp, d, cmd, cmd2);
	free(typed);
	if (tmp)
		free(tmp);
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
	free(cmd2->cmd);
	free(cmd2->args[0]);
	free(cmd->args);
	free(cmd2);
	free(cmd);
	free(cmd2->input_line);
	return (1);
}
