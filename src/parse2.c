/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 15:51:58 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/04 15:20:52 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cpy_and_slide(char **tmp, int *c, int start, t_command *cmd)
{
	while (tmp[c[0]] && !is_token(tmp[c[0]]))
	{
		if (c[0] == start)
		{
			malloc_c(&cmd->cmd, next_arg_len(tmp[c[0]]) + 2);
			ft_strlcpy(cmd->cmd, tmp[c[0]], next_arg_len(tmp[c[0]]) + 1);
		}
		if (c[0] != 0 && !ft_strncmp(cmd->cmd, "sed\0", 4))
			tmp[c[0]] = ft_strtrim(tmp[c[0]], "\'");
		malloc_c(&cmd->args[c[1]], ft_strlen(tmp[c[0]]) + 1);
		ft_strlcpy(cmd->args[c[1]], tmp[c[0]], ft_strlen(tmp[c[0]]) + 1);
		c[0]++;
		c[1]++;
	}
}

void	fill_prev(t_command *cmd, int *c, char **tmp)
{
	t_command	*prev;

	if (cmd->redir_in || cmd->redir_out)
		c[0] += 2;
	if (tmp[c[0]] && tmp[c[0]][0] == '|')
		cmd->to_pipe = 1;
	cmd->args[c[1]] = NULL;
	if (cmd->prev)
	{
		prev = cmd->prev;
		if (prev->to_pipe)
			cmd->to_pipe_to = 1;
	}
}

void	fill_next(t_command *cmd, int *c, char **tmp)
{
	t_command	*next;

	if (tmp[c[0]] && (cmd->to_pipe || cmd->to_pipe_to))
	{
		next = malloc(sizeof(t_command));
		if (!next)
			die("Malloc error");
		init_cmd(next);
		cmd->next = next;
		next->prev = cmd;
		++c[0];
		fill_cmd_fields(tmp, next, c[0]);
	}
}

char	*take_prefix(char **args)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	while (ft_strchr(args[i], '*') == NULL)
		i++;
	while (args[i][j] != '*')
		j++;
	malloc_c(&ret, i);
	ft_strlcpy(ret, args[i], j + 1);
	return (ret);
}

int	count_results(char *prefix)
{
	DIR				*stream;
	struct dirent	*entry;
	int				i;

	i = 0;
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (0);
	}
	entry = readdir(stream);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, prefix, ft_strlen(prefix)))
			i++;
		entry = readdir(stream);
	}
	closedir(stream);
	return (i);
}

char	**take_files(char *prefix)
{
	DIR				*stream;
	struct dirent	*entry;
	char			**res;
	int				i;

	i = 0;
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (NULL);
	}
	malloc_c_ptr(&res, count_results(prefix) + 1);
	entry = readdir(stream);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, prefix, ft_strlen(prefix)))
		{
			malloc_c(&res[i], ft_strlen(entry->d_name) + 1);
			ft_strlcpy(res[i], entry->d_name, ft_strlen(entry->d_name) + 1);
			i++;
		}
		entry = readdir(stream);
	}
	closedir(stream);
	res[i] = NULL;
	return (res);
}

int	count_normal_args(char **args)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (args[i])
	{
		if (ft_strchr(args[i], '*') == NULL)
			ret++;
		i++;
	}
	return (ret);
}

void	store_new_args(char **args, char **files, char **ret)
{
	int	i;
	int	j;
	int	pause;
	
	i = 0;
	j = 0;
	while (ft_strchr(args[i], '*') == NULL)
	{
		malloc_c(&ret[i], ft_strlen(args[i]) + 1);
		ft_strlcpy(ret[i], args[i], ft_strlen(args[i]) + 1);
		i++;
	}
	pause = i + 1;
	while (files[j])
	{
		malloc_c(&ret[i], ft_strlen(files[j]) + 1);
		ft_strlcpy(ret[i], files[j], ft_strlen(files[j]) + 1);
		j++;
		i++;
	}
	while (args[pause])
	{
		malloc_c(&ret[i], ft_strlen(args[i]) + 1);
		ft_strlcpy(ret[i], args[pause], ft_strlen(args[pause]) + 1);
		i++;
		pause++;
	}
	ret[i] = NULL;
}

int	treat_asterisk(t_command *cmd)
{
	int		i;
	char	*prefix;
	char	**files;
	char	**ret;

	i = 0;
	prefix = take_prefix(cmd->args);
	files = take_files(prefix);
	if (!files)
		return (-1);
	malloc_c_ptr(&ret, count_normal_args(cmd->args) + count_args(files) + 1);
	store_new_args(cmd->args, files, ret);
	free_array_of_array(cmd->args);
	malloc_c_ptr(&cmd->args, count_args(ret) + 1);
	while (ret[i])
	{
		malloc_c(&cmd->args[i], ft_strlen(ret[i]) + 1);
		ft_strlcpy(cmd->args[i], ret[i], ft_strlen(ret[i]) + 1);
		i++;
	}
	cmd->args[i] = NULL;
	free_array_of_array(ret);
	free_array_of_array(files);
	free(prefix);
	return (0);
}

int	check_asterisk(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '*') != NULL)
		{
			if (treat_asterisk(cmd) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	fill_cmd_fields(char **tmp, t_command *cmd, int start)
{
	int			c[3];

	c[2] = count_args(tmp);
	malloc_c_ptr(&cmd->args, c[2] + 1);
	c[0] = start;
	c[1] = 0;
	cpy_and_slide(tmp, c, start, cmd);
	if (tmp[c[0]])
	{
		if (check_redirection(&tmp[c[0]], cmd) == -1)
		{
			syntax_error(tmp);
			free_array_of_array(tmp);
			return (-1);
		}
	}
	fill_prev(cmd, c, tmp);
	fill_next(cmd, c, tmp);
	if (cmd->first)
		free_array_of_array(tmp);
	else
		cmd->to_pipe_to = 1;
	if (check_asterisk(cmd) == -1)
		return (-1);
	return (0);
}

int	split_command(char *line, t_command *cmd)
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
		if (fill_cmd_fields(tmp, cmd, 0) == -1)
			return (0);
	}
	return (1);
}
