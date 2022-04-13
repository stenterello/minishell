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
	int			len;
	int			i;
	t_env_var	*tmp;
	t_env_var	*new;

	new = malloc(sizeof(t_env_var));
	if (!new)
		die("Malloc error");
	len = key_len(cmd->args[1]);
	if (len == -1)
	{
		free(new);
		return ;
	}
	new->key = malloc(sizeof(char) * (len + 1));
	if (!new->key)
		die("Malloc error");
	ft_strlcpy(new->key, cmd->args[1], len + 1);
	i = len;
	len = value_len(cmd->args[1]);
	if (len == -1)
	{
		free(new->key);
		free(new);
		return ;
	}
	new->value = malloc(sizeof(char) * (len + 1));
	if (!new->value)
		die("Malloc error");
	ft_strlcpy(new->value, &cmd->args[1][i + 1], len + 1);
	new->next = NULL;
	tmp = g_term.env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	g_term.last_exit = 0;
}
