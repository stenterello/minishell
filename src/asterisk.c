#include "minishell.h"

int	treat_asterisk_prefix(t_command *cmd)
{
	int		i;
	char	*prefix;
	char	**files;
	char	**ret;

	i = 0;
	prefix = take_prefix(cmd->args);
	files = take_files_prefix(prefix);
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

int	treat_asterisk_suffix(t_command *cmd)
{
	int		i;
	char	*suffix;
	char	**files;
	char	**ret;

	i = 0;
	suffix = take_suffix(cmd->args);
	files = take_files_suffix(suffix);
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
	free(suffix);
	return (0);
}

int	treat_asterisk_all(t_command *cmd)
{
	int		i;
	char	**files;
	char	**ret;

	i = 0;
	files = take_files();
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
	return (0);
}

int	treat_asterisk_intra(t_command *cmd)
{
	int		i;
	char	*prefix;
	char	*suffix;
	char	**files;
	char	**ret;

	i = 0;
	prefix = take_prefix(cmd->args);
	suffix = take_suffix(cmd->args);
	files = take_files_intra(prefix, suffix);
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
			if (cmd->args[i][0] == '*' && ft_strlen(cmd->args[i]) == 1)
			{
				if (treat_asterisk_all(cmd) == -1)
					return (-1);
			}
			else if (cmd->args[i][0] == '*')
			{
				if (treat_asterisk_suffix(cmd) == -1)
					return (-1);
			}
			else if (ends_with_asterisk(cmd->args[i]))
			{
				if (treat_asterisk_prefix(cmd) == -1)
					return (-1);
			}
			else
			{
				if (treat_asterisk_intra(cmd) == -1)
					return (-1);
			}
		}
		i++;
	}
	return (0);
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
