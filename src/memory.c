#include "minishell.h"

void	free_dict(t_dict *env)
{
	t_dict	*tmp;
	t_dict	*tmp2;

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

void	malloc_and_check_char(char **dst, int len)
{
	*dst = malloc(sizeof(char) * len);
	if (!*dst)
		die("Malloc error");
}

void	malloc_and_check_char_ptr(char ***dst, int len)
{
	*dst = malloc(sizeof(char *) * len);
	if (!*dst)
		die("Malloc error");
}

void	malloc_and_check_dict(t_dict **dst, int len)
{
	*dst = malloc(sizeof(t_dict) * len);
	if (!*dst)
		die("Malloc error");
}

void	malloc_and_check_list(t_list **dst, int len)
{
	*dst = malloc(sizeof(t_list) * len);
	if (!*dst)
		die("Malloc error");
}
