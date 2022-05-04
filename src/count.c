/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:02 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/04 16:46:37 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_params(char *line)
{
	int	count;
	int	i;
	int	s_quot;
	int	d_quot;

	count = 1;
	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (line[i])
	{
		if (line[i] == ' ' && !d_quot && !s_quot)
			count++;
		else if (line[i] == '"' && !s_quot && !d_quot)
			d_quot = 1;
		else if (line[i] == '"' && !s_quot && d_quot)
			d_quot = 0;
		else if (line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		i++;
	}
	return (count);
}

int	count_args(char **tmp)
{
	int	i;

	i = 0;
	while (tmp[i] && !is_token(tmp[i]))
		i++;
	return (i);
}

int	next_arg_len(char *line)
{
	int	i;

	i = 0;
	while ((line[i] && line[i] != ' ' && line[i] != '>'
			&& line[i] != '<' && line[i] != '|'
			&& !is_open(line, i)) || (line[i] && is_open(line, i)))
		i++;
	if (line[0] == '<' || line[0] == '>')
		return (ft_strlen(line));
	return (i);
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

int	count_results_prefix(char *prefix)
{
	DIR				*stream;
	struct dirent	*entry;
	int				i;
	struct stat		file_stat;

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
		stat(entry->d_name, &file_stat);
		if ((file_stat.st_mode & S_IFMT) != S_IFDIR
			&& !ft_strncmp(entry->d_name, prefix, ft_strlen(prefix)))
			i++;
		entry = readdir(stream);
	}
	closedir(stream);
	return (i);
}

int	dif(char *s1, char *s2)
{
	return (ft_strlen(s1) - ft_strlen(s2));
}

int	count_results_suffix(char *suffix)
{
	DIR				*stream;
	struct dirent	*entry;
	int				i;
	struct stat		file_stat;

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
		stat(entry->d_name, &file_stat);
		if ((file_stat.st_mode & S_IFMT) != S_IFDIR
			&& dif(entry->d_name, suffix) >= 0 && !ft_strncmp(&entry->d_name[dif(entry->d_name, suffix)], suffix, ft_strlen(suffix) + 1))
			i++;
		entry = readdir(stream);
	}
	closedir(stream);
	return (i);
}

int	count_results(void)
{
	DIR				*stream;
	struct dirent	*entry;
	int				i;
	struct stat		file_stat;

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
		stat(entry->d_name, &file_stat);
		if ((file_stat.st_mode & S_IFMT) != S_IFDIR)
			i++;
		entry = readdir(stream);
	}
	closedir(stream);
	return (i);
}

int	count_results_intra(char *prefix, char *suffix)
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
		if (!ft_strncmp(entry->d_name, prefix, ft_strlen(prefix))
			&& dif(entry->d_name, suffix) >= 0
			&& !ft_strncmp(&entry->d_name[dif(entry->d_name, suffix)], suffix, ft_strlen(suffix) + 1))
			i++;
		entry = readdir(stream);
	}
	closedir(stream);
	return (i);
}
