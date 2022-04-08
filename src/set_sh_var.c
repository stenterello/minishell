#include "minishell.h"

void	insert_into_vars(char *key, char *value, t_term *term)
{
	t_sh_var	*tmp;
	t_sh_var	*new;

	if (!term->var)
	{
		term->var = malloc(sizeof(t_sh_var));
		if (!term->var)
			die("Malloc error");
		term->var->next = NULL;
		new = term->var;
		tmp = term->var;
	}
	else
	{
		new = malloc(sizeof(t_sh_var));
		if (!new)
			die("Malloc error");
		tmp = term->var;
		while (tmp->next)
			tmp = tmp->next;
	}	
	new->key = malloc(sizeof(char) * (ft_strlen(key) + 1));
	if (!new->key)
		die("Malloc error");
	ft_strlcpy(new->key, key, ft_strlen(key) + 1);
	new->value = malloc(sizeof(char) * (ft_strlen(value) + 1));
	if (!new->value)
		die("Malloc error");
	ft_strlcpy(new->value, value, ft_strlen(value) + 1);
	new->next = NULL;
	if (new == tmp)
		return ;
	tmp->next = new;
}

void	set_sh_var(char *line, t_term *term)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	i = key_len(line);
	if (i == 0)
		return ;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		die("Malloc error");
	ft_strlcpy(key, line, i + 1);
	j = i;
	i = value_len(line);
	value = malloc(sizeof(char) * (i + 1));
	if (!value)
		die("Malloc error");
	ft_strlcpy(value, &line[j + 1], i + 1);
	insert_into_vars(key, value, term);
	free(key);
	free(value);
}
