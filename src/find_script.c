/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_script.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:05:51 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/04 00:53:28 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_full_path(char *dir_name, t_command *cmd)
{
	char	*tmp;

	tmp = NULL;
	malloc_c(&tmp, ft_strlen(dir_name) + ft_strlen(cmd->cmd) + 2);
	ft_strlcpy(tmp, dir_name, ft_strlen(dir_name) + 1);
	ft_strlcat(tmp, "/", ft_strlen(dir_name) + 2);
	ft_strlcat(tmp, cmd->cmd, ft_strlen(dir_name) + ft_strlen(cmd->cmd) + 2);
	free(cmd->cmd);
	malloc_c(&cmd->cmd, ft_strlen(tmp) + 1);
	ft_strlcpy(cmd->cmd, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

int	search_in_dir(DIR *stream, t_command *cmd, char *dir_name)
{
	struct dirent	*entry;

	entry = readdir(stream);
	while (entry)
	{
		if (!ft_strncmp(entry->d_name, cmd->cmd, (ft_strlen(cmd->cmd) + 1)))
		{
			get_full_path(dir_name, cmd);
			if (!access(cmd->cmd, X_OK))
			{
				born_child(cmd);
				return (1);
			}
		}
		entry = readdir(stream);
	}
	return (0);
}

int	sup_find_script(char **path, t_command *cmd)
{
	int	i;
	int	is_exec;
	DIR	*stream;

	i = 0;
	while (path[i])
	{
		if (ft_strlen(path[i]) > 255)
			die("Path name too long");
		stream = opendir(path[i]);
		if (stream == NULL)
			die(strerror(errno));
		is_exec = search_in_dir(stream, cmd, path[i]);
		if (is_exec)
		{
			closedir(stream);
			free_array_of_array(path);
			return (0);
		}
		closedir(stream);
		i++;
	}
	return (1);
}

int	find_script(t_command *cmd)
{
	char	**path;

	path = ft_split(ft_getenv("PATH\0"), ':');
	if (path)
	{
		if (!sup_find_script(path, cmd))
			return (0);
		free_array_of_array(path);
	}
	return (-1);
}
