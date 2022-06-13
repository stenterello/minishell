/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 16:35:00 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/13 20:55:21 by ddelladi         ###   ########.fr       */
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

int	to_exp(char *str)
{
	int	i;

	i = 0;
	while (ft_strncmp(&str[i], "<<", 2))
		i++;
	i += 2;
	while (str[i] == ' ')
		i++;
	if (str[i] == '\'')
		return (0);
	return (1);
}

int	to_expand_str(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && !is_open(str, i))
			return (1);
		i++;
	}
	return (0);
}

int	key_here_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isalnum(str[i]))
		i++;
	return (i);
}

char	*take_key(char *str)
{
	int		i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (str[i] && str[i] != '\n')
	{
		if (str[i] == '$' && !is_open(str, i))
		{
			i++;
			malloc_c(&ret, key_here_len(&str[i]) + 1);
			ft_strlcpy(ret, &str[i], key_here_len(&str[i]) + 1);
			return (ret);
		}
		i++;
	}
	return (ret);
}

char	*try_expand_str(t_command *cmd)
{
	char	*ret;
	char	*key;
	int		i;

	i = 0;
	key = take_key(cmd->input_line);
	if (ft_getenv(key) != NULL)
	{
		malloc_c(&ret, ft_strlen(cmd->input_line) - ft_strlen(key) + ft_strlen(ft_getenv(key)));
		while (cmd->input_line[i] != '$')
			i++;
		ft_strlcpy(ret, cmd->input_line, i + 1);
		ft_strlcat(ret, ft_getenv(key), ft_strlen(ret) + ft_strlen(ft_getenv(key)) + 1);
		ft_strlcat(ret, &cmd->input_line[i + ft_strlen(key) + 1],
			ft_strlen(ret) + ft_strlen(&cmd->input_line[i + ft_strlen(key) + 1]) + 1);
		free(cmd->input_line);
		free(key);
		return (ret);
	}
	else
	{
		malloc_c(&ret, until_dollar(cmd->input_line) + 1);
		ft_strlcpy(ret, cmd->input_line, until_dollar(cmd->input_line));
		ft_strlcat(ret, "\n", ft_strlen(ret) + 2);
		free(cmd->input_line);
		if (key)
			free(key);
		return (ret);
	}
}

void	sup_treat(t_command *cmd, t_command *cmd2, char *typed)
{
	char	*d;
	char	*tmp;
	int		i;
	int		exp;

	i = 1;
	tmp = NULL;
	init_cmd(cmd);
	init_cmd(cmd2);
	cmd->first = 1;
	malloc_c(&cmd2->cmd, ft_strlen(typed) + 1);
	ft_strlcpy(cmd2->cmd, typed, ft_strlen(typed) + 1);
	cmd->next = cmd2;
	d = take_delimiter(cmd2->cmd);
	exp = to_exp(cmd2->cmd);
	clean_heredoc(cmd2->cmd, "<<");
	malloc_c_ptr(&cmd2->args, 2);
	malloc_c(&cmd2->args[0], ft_strlen(cmd2->cmd) + 1);
	ft_strlcpy(cmd2->args[0], cmd2->cmd, ft_strlen(cmd2->cmd) + 1);
	cmd2->args[1] = NULL;
	tmp = sup_sup_treat(tmp, i, d, cmd);
	if (exp)
	{
		while (to_expand_str(cmd->input_line))
			cmd->input_line = try_expand_str(cmd);
	}
	if (g_term.last_exit != 130)
		execute_free_here(tmp, d, cmd, cmd2);
	else
		free_here(tmp, d, cmd, cmd2);
	free(typed);
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
