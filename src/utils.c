#include "minishell.h"

void	die(char *msg)
{
	ft_putendl_fd(msg, 2);
	exit(-1);
}

int	var_name_len(char *variable)
{
	int	i;

	i = 0;
	if (!ft_strncmp(variable, "?", 1) && !ft_isalnum(variable[i + 1]))
		return (1);
	while (variable[i] && ft_isalnum(variable[i]))
		i++;
	return (i);
}

char	*search_env_vars(char *var_name)
{
	t_env_var	*tmp;
	char		*ret;

	tmp = g_term.env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, var_name, ft_strlen(var_name)))
		{
			ret = malloc(sizeof(char) * (ft_strlen(tmp->value) + 1));
			if (!ret)
				die("Malloc error");
			ft_strlcpy(ret, tmp->value, ft_strlen(tmp->value) + 1);
			return (ret);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*search_sh_vars(char *var_name)
{
	t_sh_var	*tmp;
	char		*ret;

	tmp = g_term.var;
	while (tmp && tmp->key)
	{
		if (!ft_strncmp(tmp->key, var_name, ft_strlen(var_name)))
		{
			ret = malloc(sizeof(char) * (ft_strlen(tmp->value) + 1));
			if (!ret)
				die("Malloc error");
			ft_strlcpy(ret, tmp->value, ft_strlen(tmp->value) + 1);
			return (ret);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	take_variable(char *variable, t_input *input, int init_len)
{
	int		i;
	char	*var_name;
	char	*ret;

	i = 0;
	var_name = malloc(sizeof(char) * (var_name_len(variable) + 1));
	if (!var_name)
		die("Malloc error");
	while (variable[i] && (ft_isalnum(variable[i]) || variable[i] == '?'))
	{
		var_name[i] = variable[i];
		i++;
	}
	var_name[i] = '\0';
	if (!ft_strncmp(var_name, "?", 1))
		input->expanded = ft_itoa(g_term.last_exit);
	else
		input->expanded = search_env_vars(var_name);
	if (input->expanded == NULL)
		input->expanded = search_sh_vars(var_name);
	if (input->expanded == NULL)
		input->expanded = ft_calloc(1, sizeof(char));
	ret = malloc(sizeof(char) * (ft_strlen(input->line) + 1 - i + ft_strlen(input->expanded)));
	if (!ret)
		die("Malloc error");
	ft_strlcpy(ret, input->line, init_len + 1);
	ft_strlcpy(&ret[init_len], input->expanded, ft_strlen(input->expanded) + 1);
	if ((int)ft_strlen(input->expanded) > var_name_len(variable))
	{
		if (input->line[var_name_len(variable) + 1 + init_len])
			ft_strlcpy(&ret[init_len + ft_strlen(input->expanded)], &input->line[init_len + var_name_len(variable) + 1], ft_strlen(input->line) - (init_len + var_name_len(variable)) + 1);
	}
	else
	{
		if (ft_strncmp(var_name, &input->line[var_name_len(variable) + 1 + init_len], 2))
		{
			ft_strlcpy(&ret[init_len + ft_strlen(input->expanded)], &input->line[init_len + var_name_len(variable) + 1], ft_strlen(input->line) - (init_len + var_name_len(variable)) + 1);
		}
	}
	free(var_name);
	free(input->line);
	input->line = ret;
	free(input->expanded);
}

void	clean_text(char *dst, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	dst = malloc(sizeof(char) * (ft_strlen(src)));
	if (!dst)
		die("Malloc error");
	while (src[i])
	{
		if (src[i] != '\"')
			dst[j++] = src[i++];
		else
			i++;
	}
	dst[j] = '\0';
}

int	dollar_pos(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '$')
		i++;
	return (i);
}

void	try_expand(t_input *input)
{
	int		i;
	int		s_quot;
	int		d_quot;
	char	*tmp;

	i = 0;
	s_quot = 0;
	d_quot = 0;
	while (input->line[i])
	{
		if (input->line[i] == '\'' && !s_quot && !d_quot)
			s_quot = 1;
		else if (input->line[i] == '\'' && s_quot && !d_quot)
			s_quot = 0;
		if (input->line[i] == '"' && !s_quot && !d_quot)
			d_quot = 1;
		else if (input->line[i] == '"' && !s_quot && d_quot)
			d_quot = 0;
		if (input->line[i] == '$' && !s_quot)
		{
			i++;
			if (ft_isalpha(input->line[i]) || input->line[i] == '?')
			{
				if (d_quot)
				{
					tmp = malloc(sizeof(char) * (ft_strlen(input->line) + 1));
					if (!tmp)
						die("Malloc error");
					ft_strlcpy(tmp, input->line, ft_strlen(input->line) + 1);
					free(input->line);
					// i = dollar_pos(input->line) + 1;
					clean_text(input->line, tmp);
					// take_variable(var, input, i - 1);
					free(tmp);
				}
				else
					take_variable(&input->line[i], input, i - 1);
			}
			return ;
		}
		i++;
	}
}

char	*get_path(char *line)
{
	char	*ret;
	int		i;

	i = 0;
	while (line[i] == 32 || (line[i] > 8 && line[i] < 14))
		i++;
	ret = malloc(sizeof(char) * (ft_strlen(line) - i + 1));
	if (!ret)
		die("Malloc error trying to get absolute path [CD]");
	ft_strlcpy(ret, &line[i], ft_strlen(line) - i + 1);
	return (ret);
}
