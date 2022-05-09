#include "minishell.h"

int	ft_islower(char c)
{
	if (97 <= c && c <= 122)
		return (1);
	return (0);
}

int	is_wildcard(char c)
{
	if (c == '*' || c == '?' || c == '[')
		return (1);
	return (0);
}

int	count_portions(char *line)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (line[i])
	{
		if (line[i] == '[' && !is_open(line, i))
		{
			ret++;
			while (line[i] && line[i] != ']')
				i++;
			if (is_open(line, i))
				return (-1);
		}
		else if (!is_open(line, i) && is_wildcard(line[i]))
		{
			ret++;
		}
		else if (ft_isalnum(line[i]))
		{
			ret++;
			while (line[i] && !is_wildcard(line[i]))
				i++;
			i--;
		}
		i++;
	}
	return (ret);
}

int	try_parse_brackets(char *line)
{
	int		i;

	i = 0;
	if (line[i] == '[')
		i++;
	else
		return (-1);
	while (ft_isascii(line[i]) && line[i] != ']' && line[i])
		i++;
	if (line[i] != ']')
		return (-1);
	return (i + 1);
}

void	insert_brackets_param(char *line, char *portion, int len)
{
	int		i;
	char	*tmp;

	i = 0;
	malloc_c(&tmp, len + 1);
	while (line[i] && ft_isascii(line[i]) && line[i] != ']')
	{
		tmp[i] = line[i];
		i++;
	}
	tmp[i++] = ']';
	tmp[i] = '\0';
	ft_strlcpy(portion, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

int	take_brackets_param(char *line, char **portion)
{
	if (try_parse_brackets(line) != -1)
	{
		malloc_c(portion, try_parse_brackets(line) + 1);
		insert_brackets_param(line, *portion, try_parse_brackets(line));
		return (1);
	}
	return (-1);
}

void	take_string_portion(char *line, char **portion)
{
	int	i;

	i = 0;
	while (line[i] && !is_wildcard(line[i]))
		i++;
	malloc_c(portion, i + 1);
	ft_strlcpy(*portion, line, i + 1);
}

// int	find_next_bracket_occurrence(char *portion, char *file)
// {}

int	count_range(char a, char b)
{
	int	i;

	i = a + 1;
	while (i < b)
		i++;
	return (i - a + 1);
}

int	count_letters(char *brackets)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (brackets[i] != ']')
	{
		if (brackets[i] == '-' && count_range(brackets[i - 1], brackets[i + 1]) != -1)
			ret += count_range(brackets[i - 1], brackets[i + 1]);
		else if (count_range(brackets[i - 1], brackets[i + 1]) == -1)
			return (-1);
		i++;
		if (ft_strchr("!^-]", brackets[i]) == NULL)
			ret++;
	}
	return (ret);
}

char	*chrs_range(char a, char b)
{
	char	*tmp;
	int		i;
	int		j;

	i = a + 1;
	j = 0;
	tmp = malloc(sizeof(char) * (b - a + 1));
	while (i < b)
		tmp[j++] = (char)i++;
	tmp[j] = '\0';
	return (tmp);
}

char	*get_letters(char *brackets)
{
	int		i;
	int		len;
	char	*ret;
	char	*tmp;

	i = 1;
	len = 2;
	ret = malloc(sizeof(char) * count_letters(brackets) + 1);
	while (brackets[i] != ']')
	{
		if (brackets[i] != '-')
		{
			if (i == 1)
				ft_strlcpy(ret, &brackets[i], len++);
			else
				ft_strlcat(ret, &brackets[i], len++);
		}
		else
		{
			tmp = chrs_range(brackets[i - 1], brackets[i + 1]);
			len += ft_strlen(tmp) - 1;
			ft_strlcat(ret, tmp, len++);
			free(tmp);
		}
		i++;
	}
	return (ret);
}

int	is_in(char *range, char c)
{
	int	i;

	i = 0;
	while (range[i])
	{
		if (c == range[i])
			return (1);
		i++;
	}
	return (0);
}

int	is_verified_brackets(char *brackets, char file_char)
{
	char	*rules;

	if (count_letters(brackets) == -1)
		return (-1);
	rules = get_letters(brackets);
	if (is_in(rules, file_char))
	{
		free(rules);
		return (1);
	}
	free(rules);
	return (0);
}

int	find_brackets_occurrence(char *file, char *portion, int *f_ind)
{

}

int	question_mark(char *file, char *portion, int *f_ind)
{
	if (file[++(*f_ind)])
		return (1);
	else
		return (0);
}

int	treat_init_asterisk(char *file, char **portions, int *p_ind, int *f_ind)
{
	int	ret;

	ret = 0;
	while (portions[*p_ind] && portions[*p_ind][0] == '*')
		*p_ind++;
	if (!portions[*p_ind])
		return (1);
	while (portions[*p_ind] && file[*f_ind])
	{
		if (portions[*p_ind][0] == '[')
			ret = find_brackets_occurrence(file, portions[*p_ind], f_ind);
		else if (portions[*p_ind][0] == '?')
			ret = question_mark(file, portions[*p_ind], f_ind);
		else if (portions[*p_ind][0] == '*')
			ret = treat_init_asterisk(file, portions, p_ind, f_ind);
		if (!ret)
			return (ret);
		*p_ind++;
	}
	return (ret);
}

int	is_verified(char *file, char **portions)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (portions[++i])
	{
		if (portions[i][0] == '*')
			return (treat_init_asterisk(file, portions, &i, &j));
	}
}

int	count_results(char **portions)
{
	DIR				*stream;
	int				ret;
	struct dirent	*entry;

	ret = 0;
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	entry = readdir(stream);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, ".", 1) && is_verified(entry->d_name, portions) > 0)
			ret++;
		else if (is_verified(entry->d_name, portions) == -1)
			return (-1);
		else if (is_verified(entry->d_name, portions) == 0)
			return (0);
		entry = readdir(stream);
	}
	closedir(stream);
	return (ret);
}

char	**get_results(char **portions, int len)
{
	DIR				*stream;
	char			**ret;
	int				i;
	struct dirent	*entry;

	i = 0;
	malloc_c_ptr(&ret, len + 1);
	stream = opendir(".");
	if (!stream)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (NULL);
	}
	entry = readdir(stream);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, ".", 1) && is_verified(entry->d_name, portions) > 0)
		{
			malloc_c(&ret[i], ft_strlen(entry->d_name) + 1);
			ft_strlcpy(ret[i++], entry->d_name, ft_strlen(entry->d_name) + 1);
		}
		entry = readdir(stream);
	}
	ret[i] = NULL;
	closedir(stream);
	return (ret);
}

int	has_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '?' || str[i] == '*' || str[i] == '[') && !is_open(str, i))
			return (1);
		i++;
	}
	return (0);
}

void	substitute_args(t_command *cmd, char **tmp)
{
	int		j;
	int		i;
	int		i2;
	int		len;
	char	**helper;

	len = count_args(cmd->args) + count_args(tmp);
	malloc_c_ptr(&helper, len + 1);
	j = 0;
	while (cmd->args[j] && !has_wildcard(cmd->args[j]))
	{
		malloc_c(&helper[j], ft_strlen(cmd->args[j]) + 1);
		ft_strlcpy(helper[j], cmd->args[j], ft_strlen(cmd->args[j]) + 1);
		j++;
	}
	i2 = 0;
	i = j + 1;
	while (tmp[i2])
	{
		malloc_c(&helper[j], ft_strlen(tmp[i2]) + 1);
		ft_strlcpy(helper[j++], tmp[i2], ft_strlen(tmp[i2]) + 1);
		i2++;
	}
	while (cmd->args[i])
	{
		malloc_c(&helper[j], ft_strlen(cmd->args[i]) + 1);
		ft_strlcpy(helper[j++], cmd->args[i], ft_strlen(cmd->args[i]) + 1);
		i++;
	}
	helper[j] = NULL;
	free_array_of_array(cmd->args);
	cmd->args = helper;
}

int	guess(t_command *cmd, int i)
{
	int		j;
	int		k;
	char	**tmp;

	j = 0;
	k = 0;
	malloc_c_ptr(&cmd->portions, count_portions(cmd->args[i]) + 1);
	while (cmd->args[i][j])
	{
		if (!is_open(cmd->args[i], j) && (cmd->args[i][j] == '*' || cmd->args[i][j] == '?'))
		{
			malloc_c(&cmd->portions[k], 2);
			ft_strlcpy(cmd->portions[k], &cmd->args[i][j], 2);
			k++;
		}
		else if (!is_open(cmd->args[i], j) && cmd->args[i][j] == '[')
		{
			if (take_brackets_param(&cmd->args[i][j], &cmd->portions[k]) == -1)
			{
				free_array_of_array(cmd->portions);
				return (-1);
			}
			k++;
			j += try_parse_brackets(&cmd->args[i][j]) - 1;
		}
		else
		{
			take_string_portion(&cmd->args[i][j], &cmd->portions[k]);
			j += ft_strlen(cmd->portions[k++]) - 1;
		}
		j++;
	}
	cmd->portions[k] = NULL;
	if (count_results(cmd->portions) == -1)
	{
		free_array_of_array(cmd->portions);
		return (-1);
	}
	else if (count_results(cmd->portions) == 0)
		return (0);
	tmp = get_results(cmd->portions, count_results(cmd->portions));
	if (!tmp)
		return (-1);
	substitute_args(cmd, tmp);
	free_array_of_array(tmp);
	return (1);
}

int	check_wildcards(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (has_wildcard(cmd->args[i]))
		{
			if (guess(cmd, i) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
