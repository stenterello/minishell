/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:06:02 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/20 15:47:29 by ddelladi         ###   ########.fr       */
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

char	*try_expand_str(t_command *cmd, t_terminfo *terminfo)
{
	char	*ret;
	char	*key;
	int		i;

	i = 0;
	key = take_key(cmd->input_line);
	malloc_c(&ret, ft_strlen_rl(cmd->input_line)
		- ft_strlen(key) + ft_strlen(ft_getenv(key, terminfo)));
	while (cmd->input_line[i] != '$')
		i++;
	ft_strlcpy(ret, cmd->input_line, i + 1);
	if (ft_getenv(key, terminfo))
		ft_strlcat(ret, ft_getenv(key, terminfo),
			ft_strlen(ret) + ft_strlen(ft_getenv(key, terminfo)) + 1);
	ft_strlcat(ret, &cmd->input_line[i + ft_strlen(key) + 1],
		ft_strlen(ret) + ft_strlen_rl(&cmd->input_line
		[i + ft_strlen(key) + 1]) + 1);
	free(cmd->input_line);
	if (key)
		free(key);
	return (ret);
}

static char	*prepare_heredoc(t_command *cmd, t_command *cmd2,
	char *typed, int *exp)
{
	char	*ret;

	init_cmd(cmd);
	init_cmd(cmd2);
	cmd->first = 1;
	malloc_c(&cmd2->cmd, ft_strlen(typed) + 1);
	ft_strlcpy(cmd2->cmd, typed, ft_strlen(typed) + 1);
	cmd2->next = cmd;
	ret = take_delimiter(cmd2->cmd);
	*exp = to_exp(cmd2->cmd);
	clean_heredoc(cmd2->cmd, "<<");
	malloc_c_ptr(&cmd2->args, 2);
	malloc_c(&cmd2->args[0], ft_strlen(cmd2->cmd) + 1);
	ft_strlcpy(cmd2->args[0], cmd2->cmd, ft_strlen(cmd2->cmd) + 1);
	cmd2->args[1] = NULL;
	return (ret);
}

static void	treat_heredoc(t_command *cmd, t_command *cmd2,
	char *typed, t_terminfo *terminfo)
{
	char	*d;
	char	*tmp;
	int		exp;

	tmp = NULL;
	d = prepare_heredoc(cmd, cmd2, typed, &exp);
	//d = take_delimiter(cmd2->cmd);
	tmp = take_heredoc_input(tmp, d, cmd, terminfo);
	if (exp && !is_open(typed, ft_strlen(typed)))
	{
		while (to_expand_str(cmd->input_line))
			cmd->input_line = try_expand_str(cmd, terminfo);
	}
	if (terminfo->last_exit != 130)
	{
		free(d);
		execute_free_here(tmp, cmd, cmd2, terminfo);
	}
	else
		free_here(tmp, d, cmd, cmd2);
	free(typed);
}

int	init_heredoc(char *typed, t_terminfo *terminfo)
{
	t_command	*cmd;
	t_command	*cmd2;

	terminfo->delimiter = 1;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		die("Malloc error");
	cmd2 = malloc(sizeof(t_command));
	if (!cmd2)
		die("Malloc error");
	treat_heredoc(cmd, cmd2, typed, terminfo);
	return (1);
}
