#include "../minishell.h"

void	sup_sup_cd1(t_command *old, char *act)
{
	int	i;

	g_term.last_exit = 0;
	malloc_c_ptr(&old->args, 3);
	malloc_c(&old->args[0], 2);
	ft_strlcpy(old->args[0], "A", 2);
	malloc_c(&old->args[1], ft_strlen("OLDPWD=") + ft_strlen(act) + 1);
	ft_strlcpy(old->args[1], "OLDPWD=", ft_strlen("OLDPWD=") + 1);
	ft_strlcat(old->args[1], act,
		ft_strlen(old->args[1]) + ft_strlen(act) + 1);
	old->args[2] = NULL;
	export(old);
	i = 0;
	while (old->args[i])
		free(old->args[i++]);
	free(old->args);
	free(act);
	free(old);
}

void	sup_sup_cd2(t_command *old, char *act)
{
	int	i;

	g_term.last_exit = 0;
	malloc_c_ptr(&old->args, 3);
	malloc_c(&old->args[0], 2);
	ft_strlcpy(old->args[0], "A", 2);
	malloc_c(&old->args[1], ft_strlen("OLDPWD=") + ft_strlen(act) + 1);
	ft_strlcpy(old->args[1], "OLDPWD=", ft_strlen("OLDPWD=") + 1);
	ft_strlcat(old->args[1], act,
		ft_strlen(old->args[1]) + ft_strlen(act) + 1);
	old->args[2] = NULL;
	export(old);
	i = 0;
	while (old->args[i])
		free(old->args[i++]);
	free(old->args);
	free(act);
	free(old);
}

void	sup_sup_cd3(char *act, t_command *old)
{
	int	i;

	g_term.last_exit = 0;
	malloc_c_ptr(&old->args, 3);
	malloc_c(&old->args[0], 2);
	ft_strlcpy(old->args[0], "A", 2);
	malloc_c(&old->args[1], ft_strlen("OLDPWD=") + ft_strlen(act) + 1);
	ft_strlcpy(old->args[1], "OLDPWD=", ft_strlen("OLDPWD=") + 1);
	ft_strlcat(old->args[1], act,
		ft_strlen(old->args[1]) + ft_strlen(act) + 1);
	old->args[2] = NULL;
	export(old);
	i = 0;
	while (old->args[i])
		free(old->args[i++]);
	free(old->args);
	free(act);
	free(old);
}
