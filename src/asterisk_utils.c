#include "minishell.h"

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

char	*take_suffix(char **args)
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
	j++;
	malloc_c(&ret, ft_strlen(args[i]) - j + 1);
	ft_strlcpy(ret, &args[i][j], ft_strlen(args[i]) - j + 1);
	return (ret);
}

char	**take_files(void)
{
	DIR				*stream;
	struct dirent	*entry;
	char			**res;
	int				i;
	struct stat		file_stat;

	i = 0;
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (NULL);
	}
	malloc_c_ptr(&res, count_results() + 1);
	entry = readdir(stream);
	while (entry)
	{
		stat(entry->d_name, &file_stat);
		if ((file_stat.st_mode & S_IFMT) != S_IFDIR)
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

char	**take_files_prefix(char *prefix)
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
	malloc_c_ptr(&res, count_results_prefix(prefix) + 1);
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

char	**take_files_suffix(char *suffix)
{
	DIR				*stream;
	struct dirent	*entry;
	char			**res;
	int				i;
	struct stat		file_stat;

	i = 0;
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (NULL);
	}
	malloc_c_ptr(&res, count_results_suffix(suffix) + 1);
	entry = readdir(stream);
	while (entry)
	{
		stat(entry->d_name, &file_stat);
		if (((file_stat.st_mode & S_IFMT) != S_IFDIR)
			&& dif(entry->d_name, suffix) >= 0 && !ft_strncmp(&entry->d_name[dif(entry->d_name, suffix)], suffix, ft_strlen(suffix) + 1))
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

char	**take_files_intra(char *prefix, char *suffix)
{
	DIR				*stream;
	struct dirent	*entry;
	char			**res;
	int				i;
	struct stat		file_stat;

	i = 0;
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (NULL);
	}
	malloc_c_ptr(&res, count_results_intra(prefix, suffix) + 1);
	entry = readdir(stream);
	while (entry)
	{
		stat(entry->d_name, &file_stat);
		if (((file_stat.st_mode & S_IFMT) != S_IFDIR)
			&& !ft_strncmp(entry->d_name, prefix, ft_strlen(prefix))
			&& dif(entry->d_name, suffix) >= 0
			&& !ft_strncmp(&entry->d_name[dif(entry->d_name, suffix)], suffix, ft_strlen(suffix) + 1))
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
