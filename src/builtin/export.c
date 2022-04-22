#include "../minishell.h"

int	key_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	if (!line[i])
		return (-1);
	return (i);
}

int	value_len(char *line)
{
	int	i;
	int	j;

	i = key_len(line) + 1;
	if (line[i] == ' ')
		return (0);
	while (line[i] == '\"' || line[i] == '\'')
		i++;
	j = i;
	while (line[i] && line[i] != '"' && line[i] != '\'')
		i++;
	if (i - j == 0)
		return (-1);
	return (i - j);
}

void	export(t_command *cmd)
{
	int		len;
	int		i;
	t_dict	*new;

	new = NULL;
	malloc_and_check_dict(&new, 1);
	len = key_len(cmd->args[1]);
	if (len == -1)
	{
		free(new);
		return ;
	}
	malloc_and_check_char(&new->key, len + 1);
	ft_strlcpy(new->key, cmd->args[1], len + 1);
	i = len;
	len = value_len(cmd->args[1]);
	if (len == -1)
	{
		free(new->key);
		free(new);
		return ;
	}
	malloc_and_check_char(&new->value, len + 1);
	ft_strlcpy(new->value, &cmd->args[1][i + 1], len + 1);
	new->next = NULL;
	if (!change_exist_var_in_dict(new->key, new->value, g_term.env))
		insert_into_vars(new->key, new->value, g_term.env);
	free(new->key);
	free(new->value);
	free(new);
	g_term.last_exit = 0;
}
