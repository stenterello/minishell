#include "minishell.h"

void	insert_into_vars(char *key, char *value)
{
	t_sh_var	*tmp;
	t_sh_var	*new;

	tmp = g_term.var;
	while (tmp->next)
		tmp = tmp->next;
	if (tmp->key)
	{
		new = malloc(sizeof(t_sh_var));
		if (!new)
			die("Malloc error");
		tmp->next = new;
	}
	else
		new = tmp;
	new->key = malloc(sizeof(char) * (ft_strlen(key) + 1));
	if (!new->key)
		die("Malloc error");
	ft_strlcpy(new->key, key, ft_strlen(key) + 1);
	new->value = malloc(sizeof(char) * (ft_strlen(value) + 1));
	if (!new->value)
		die("Malloc error");
	ft_strlcpy(new->value, value, ft_strlen(value) + 1);
	new->next = NULL;
}

int	change_exist_var(char *key, char *value)
{
	t_sh_var	*tmp;

	if (!g_term.var)
	{
		g_term.var = malloc(sizeof(t_sh_var));
		if (!g_term.var)
			die("Malloc error");
		tmp = g_term.var;
		tmp->key = NULL;
	}
	else
		tmp = g_term.var;
	while (tmp && tmp->key)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key)))
		{
			free(tmp->value);
			tmp->value = malloc(sizeof(char) * (ft_strlen(value) + 1));
			if (!tmp->value)
				die("Malloc error");
			ft_strlcpy(tmp->value, value, ft_strlen(value) + 1);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	change_exist_env(char *key, char *value)
{
	t_env_var	*tmp;

	if (!g_term.env)
	{
		g_term.env = malloc(sizeof(t_env_var));
		if (!g_term.env)
			die("Malloc error");
		tmp = g_term.env;
		tmp->key = NULL;
	}
	else
		tmp = g_term.env;
	while (tmp && tmp->key)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key)))
		{
			free(tmp->value);
			tmp->value = malloc(sizeof(char) * (ft_strlen(value) + 1));
			if (!tmp->value)
				die("Malloc error");
			ft_strlcpy(tmp->value, value, ft_strlen(value) + 1);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	quoted(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '=')
		i++;
	i++;
	if (line[i] == '\'' || line[i] == '"')
		return (1);
	return (0);
}

void	set_sh_var(char **args)
{
	int		i;
	int		j;
	int		k;
	char	*key;
	char	*value;

	k = 0;
	while (args[k])
	{
		i = key_len(args[k]);
		if (i == 0)
			return ;
		key = malloc(sizeof(char) * (i + 1));
		if (!key)
			die("Malloc error");
		ft_strlcpy(key, args[k], i + 1);
		j = i;
		i = value_len(args[k]);
		value = malloc(sizeof(char) * (i + 1));
		while (args[k][j] == '\"' || args[k][j] == '\'')
			j++;
		if (!value)
			die("Malloc error");
		if (quoted(args[k]))
			ft_strlcpy(value, &args[k][j + 2], i + 1);
		else
			ft_strlcpy(value, &args[k][j + 1], i + 1);
		if (!change_exist_env(key, value))
		{
			if (!change_exist_var(key, value))
				insert_into_vars(key, value);
		}
		free(key);
		free(value);
		k++;
	}
}
