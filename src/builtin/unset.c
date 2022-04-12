#include "../minishell.h"

void	delete_env_var(t_env_var *tmp, int *flag)
{
	free(tmp->key);
	free(tmp->value);
	free(tmp);
	*flag = 1;
}

void	delete_sh_var(t_sh_var *tmp, int *flag)
{
	free(tmp->key);
	free(tmp->value);
	free(tmp);
	*flag = 1;
}

void	search_and_delete(char *search, t_term *term)
{
	t_env_var	*tmp;
	t_env_var	*tmp2;
	t_sh_var	*tmp3;
	t_sh_var	*tmp4;
	int			flag;

	flag = 0;
	tmp = term->env;
	if (!ft_strncmp(tmp->key, search, ft_strlen(search)))
	{
		term->env = tmp->next;
		delete_env_var(tmp, &flag);
	}
	while (tmp->next && !flag)
	{
		tmp2 = tmp->next;
		if (!ft_strncmp(tmp2->key, search, ft_strlen(search)))
		{
			tmp->next = tmp2->next;
			delete_env_var(tmp2, &flag);
			break ;
		}
		tmp = tmp->next;
	}
	if (!flag && term->var)
	{
		tmp3 = term->var;
		if (!ft_strncmp(tmp3->key, search, ft_strlen(search)))
		{
			term->var = tmp3->next;
			delete_sh_var(tmp3, &flag);
		}
		while (tmp3->next && !flag)
		{
			tmp4 = tmp3->next;
			if (!ft_strncmp(tmp4->key, search, ft_strlen(search)))
			{
				tmp->next = tmp4->next;
				delete_sh_var(tmp4, &flag);
				break ;
			}
			tmp3 = tmp3->next;
		}
	}
}

void	unset(t_command *cmd, t_term *term)
{
	int		i;
	char	*search;

	i = ft_strlen(cmd->args[0]);
	if (i == 0)
		return ;
	search = malloc(sizeof(char) * (i + 1));
	if (!search)
		die("Malloc error");
	ft_strlcpy(search, cmd->args[0], i + 1);
	search_and_delete(search, term);
	free(search);
	term->last_exit = 0;
}
