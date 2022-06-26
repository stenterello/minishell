/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:06:02 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/26 18:45:50 by ddelladi         ###   ########.fr       */
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

int	more_args_heredoc(char *line)
{
	int	i;

	i = skip_spaces(line, delimiter_len(&line[0]));
	if (!ft_strncmp(&line[i], "<<", 2))
	{
		while (another_heredoc(&line[i]))
		{
			i = skip_spaces(line, i + 2)
				+ delimiter_len(&line[skip_spaces(line, i + 2)]);
			i = skip_spaces(line, i);
		}
		return (0);
	}
	else
	{
		while (line[i] && line[i] != '<')
		{
			i += delimiter_len(&line[i]);
			i = skip_spaces(line, i);
		}
		i = skip_spaces(line, i + 2)
			+ delimiter_len(&line[skip_spaces(line, i + 2)]);
		i = skip_spaces(line, i);
		return (1);
	}
}

static char	*prepare_heredoc(t_command *cmd2,
	char *typed, int *exp, t_terminfo *terminfo)
{
	char		*ret;
	t_command	*cmd;

	cmd = (t_command *)cmd2->next;
	malloc_c(&cmd2->cmd, ft_strlen(typed) + 1);
	ft_strlcpy(cmd2->cmd, typed, ft_strlen(typed) + 1);
	cmd2->next = cmd;
	ret = take_delimiter(cmd2->cmd, terminfo);
	exp[0] = to_exp(cmd2->cmd);
	exp[1] = more_args_heredoc(cmd2->cmd);
	clean_heredoc(cmd2);
	if (!cmd2->args)
	{
		malloc_c_ptr(&cmd2->args, 2);
		cmd2->args[1] = NULL;
	}
	malloc_c(&cmd2->args[0], ft_strlen(cmd2->cmd) + 1);
	ft_strlcpy(cmd2->args[0], cmd2->cmd, ft_strlen(cmd2->cmd) + 1);
	return (ret);
}

void	treat_heredoc(t_command *cmd, t_command *cmd2,
	char *typed, t_terminfo *terminfo)
{
	char	*d;
	char	*tmp;
	int		exp[2];

	tmp = NULL;
	add_signals(terminfo, cmd);
	d = prepare_heredoc(cmd2, typed, exp, terminfo);
	tmp = take_heredoc_input(tmp, d, cmd, terminfo);
	while (exp[0] && !is_open(typed, ft_strlen(typed))
		&& to_expand_str(cmd->input_line))
		cmd->input_line = try_expand_str(cmd, terminfo);
	if (terminfo->last_exit != 130 && !exp[1])
		execute_free_here(tmp, cmd2, d, terminfo);
	else if (terminfo->last_exit != 130 && exp[1])
	{
		cmd2->next = NULL;
		terminfo->delimiter = 0;
		execute_tree(cmd2, terminfo);
		free_here(tmp, d, cmd, cmd2);
	}
	else
		free_here(tmp, d, cmd, cmd2);
	free(typed);
}
