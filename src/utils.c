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
	while (variable[i])
		i++;
	return (i);
}

char	*search_env_vars(char *var_name, t_term *term)
{
	t_env_var	*tmp;

	tmp = term->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, var_name, ft_strlen(var_name)))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*search_sh_vars(char *var_name, t_term *term)
{
	t_sh_var	*tmp;

	tmp = term->var;
	while (tmp && tmp->key)
	{
		if (!ft_strncmp(tmp->key, var_name, ft_strlen(var_name)))
			return (tmp->value);
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
	while (variable[i])
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

void	try_expand(t_input *input, t_term *term)
{
	int	i;

	i = 0;
	while (input->line[i])
	{
		if (input->line[i] == '$')
		{
			i++;
			if (ft_isalpha(input->line[i]) || input->line[i] == '?')
				take_variable(&input->line[i], input, i - 1, term);
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
