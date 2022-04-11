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
	while (variable[i] && variable[i] != ' ')
		i++;
	return (i);
}

char	*search_env_vars(char *var_name, t_term *term)
{
	t_env_var	*tmp;
	char		*ret;

	tmp = term->env;
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

char	*search_sh_vars(char *var_name, t_term *term)
{
	t_sh_var	*tmp;
	char		*ret;

	tmp = term->var;
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

void	take_variable(char *variable, t_input *input, int init_len, t_term *term)
{
	int		i;
	char	*var_name;
	char	*ret;

	i = 0;
	var_name = malloc(sizeof(char) * (var_name_len(variable) + 1));
	if (!var_name)
		die("Malloc error");
	while (variable[i] && variable[i] != ' ')
	{
		var_name[i] = variable[i];
		i++;
	}
	var_name[i] = '\0';
	if (!ft_strncmp(var_name, "?", 1))
		input->expanded = ft_itoa(term->last_exit);
	else
		input->expanded = search_env_vars(var_name, term);
	if (input->expanded == NULL)
		input->expanded = search_sh_vars(var_name, term);
	if (input->expanded == NULL)
		input->expanded = ft_calloc(1, sizeof(char));
	free(var_name);
	ret = malloc(sizeof(char) * (ft_strlen(input->line) + 1 - i + ft_strlen(input->expanded)));
	if (!ret)
		die("Malloc error");
	ft_strlcpy(ret, input->line, init_len + 1);
	ft_strlcpy(&ret[init_len], input->expanded, ft_strlen(input->expanded) + 1);
	if (input->line[var_name_len(variable) + 1 + init_len])
		ft_strlcpy(&ret[init_len + ft_strlen(input->expanded)], &input->line[init_len + var_name_len(variable) + 1], ft_strlen(input->line) - (init_len + var_name_len(variable)) + 1);
	free(input->line);
	input->line = ret;
	free(input->expanded);
}

char	*clean_text(char *line)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	tmp = malloc(sizeof(char) * (i + 1));
	if (!tmp)
		die("Malloc error");
	ft_strlcpy(tmp, line, i + 1);
	tmp2 = ft_strtrim(tmp, "\" ");
	free(tmp);
	return (tmp2);
}

void	try_expand(t_input *input, t_term *term)
{
	int		i;
	int		s_quot;
	int		d_quot;
	char	*var;

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
					var = clean_text(&input->line[i - 2]);
					take_variable(&var[1], input, i - 1, term);
					free(var);
				}
				else
					take_variable(&input->line[i], input, i - 1, term);
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
