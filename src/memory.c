#include "minishell.h"

void	free_env(t_env_var *env)
{
	t_env_var	*tmp;
	t_env_var	*tmp2;

	tmp = env;
	while (tmp)
	{
		tmp2 = tmp;
		free(tmp2->key);
		free(tmp2->value);
		tmp = tmp->next;
		free(tmp2);
	}
}

void	free_sh(t_sh_var *var)
{
	t_sh_var	*tmp;
	t_sh_var	*tmp2;

	tmp = var;
	while (tmp)
	{
		tmp2 = tmp;
		free(tmp2->key);
		free(tmp2->value);
		tmp = tmp->next;
		free(tmp2);
	}
}
