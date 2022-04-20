#include "../minishell.h"

void	env(void)
{
	t_dict	*tmp;

	tmp = g_term.env;
	while (tmp)
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(tmp->value, 1);
		tmp = tmp->next;
	}
	g_term.last_exit = 0;
}
