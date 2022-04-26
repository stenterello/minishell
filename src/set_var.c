#include "minishell.h"

void	insert_into_vars(char *key, char *value, t_dict *where)
{
	t_dict	*tmp;
	t_dict	*new;

	tmp = where;
	new = NULL;
	while (tmp->next)
		tmp = tmp->next;
	if (tmp->key)
	{
		malloc_and_check_dict(&new, 1);
		tmp->next = new;
	}
	else
		new = tmp;
	malloc_and_check_char(&new->key, ft_strlen(key) + 1);
	ft_strlcpy(new->key, key, ft_strlen(key) + 1);
	malloc_and_check_char(&new->value, ft_strlen(value) + 1);
	ft_strlcpy(new->value, value, ft_strlen(value) + 1);
	new->next = NULL;
}

int	change_exist_var_in_dict(char *key, char *value, t_dict *where)
{
	t_dict	*tmp;

	if (!where)
	{
		malloc_and_check_dict(&where, 1);
		tmp = where;
		tmp->key = NULL;
	}
	else
		tmp = where;
	while (tmp && tmp->key)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1))
		{
			free(tmp->value);
			malloc_and_check_char(&tmp->value, ft_strlen(value) + 1);
			ft_strlcpy(tmp->value, value, ft_strlen(value) + 1);
			return (1);
		}
		tmp = tmp->next;
	}
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
	key = NULL;
	value = NULL;
	while (args[k])
	{
		i = key_len(args[k]);
		if (i == 0)
			return ;
		malloc_and_check_char(&key, i + 1);
		ft_strlcpy(key, args[k], i + 1);
		j = i;
		i = value_len(args[k]);
		malloc_and_check_char(&value, i + 1);
		while (args[k][j] == '\"' || args[k][j] == '\'')
			j++;
		if (quoted(args[k]))
			ft_strlcpy(value, &args[k][j + 2], i + 1);
		else
			ft_strlcpy(value, &args[k][j + 1], i + 1);
		if (!change_exist_var_in_dict(key, value, g_term.env))
		{
			if (!change_exist_var_in_dict(key, value, g_term.var))
				insert_into_vars(key, value, g_term.var);
		}
		free(key);
		free(value);
		k++;
	}
}

void	treat_var_decl(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (!is_var_def(cmd->args[i]))
		{
			rewrite_args(cmd);
			execute_tree(cmd);
			return ;
		}
		i++;
	}
	set_sh_var(cmd->args);
	i = 0;
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	free(cmd->cmd);
}
