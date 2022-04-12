#include "../minishell.h"

void	env(t_term *term)
{
	t_env_var	*tmp;

	tmp = term->env;
	while (tmp)
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(tmp->value, 1);
		tmp = tmp->next;
	}
	term->last_exit = 0;
}
