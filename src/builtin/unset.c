#include "../minishell.h"

void	delete_dict_var(t_dict *tmp, int *flag)
{
	free(tmp->key);
	free(tmp->value);
	free(tmp);
	*flag = 1;
}

void	search_and_delete(char *search)
{
	t_dict	*tmp;
	t_dict	*tmp2;
	int		flag;

	flag = 0;
	tmp = g_term.env;
	if (!ft_strncmp(tmp->key, search, ft_strlen(search)))
	{
		g_term.env = tmp->next;
		delete_dict_var(tmp, &flag);
	}
	while (tmp && !flag && tmp->next)
	{
		tmp2 = tmp->next;
		if (!ft_strncmp(tmp2->key, search, ft_strlen(search)))
		{
			tmp->next = tmp2->next;
			delete_dict_var(tmp2, &flag);
			break ;
		}
		tmp = tmp->next;
	}
	if (!flag && g_term.var->key)
	{
		tmp = g_term.var;
		if (!ft_strncmp(tmp->key, search, ft_strlen(search)))
		{
			g_term.var = tmp->next;
			delete_dict_var(tmp, &flag);
		}
		while (tmp->next && !flag)
		{
			tmp2 = tmp->next;
			if (!ft_strncmp(tmp2->key, search, ft_strlen(search)))
			{
				tmp->next = tmp2->next;
				delete_dict_var(tmp2, &flag);
				break ;
			}
			tmp = tmp->next;
		}
	}
}

void	unset(t_command *cmd)
{
	int		i;
	char	*search;

	search = NULL;
	i = ft_strlen(cmd->args[1]);
	if (i == 0)
		return ;
	malloc_and_check_char(&search, i + 1);
	ft_strlcpy(search, cmd->args[1], i + 1);
	search_and_delete(search);
	free(search);
	g_term.last_exit = 0;
}
