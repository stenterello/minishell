/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:17 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 15:06:43 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	ft_putendl_fd(act, 1);
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

void	update_pwd(void)
{
	char	*ret;

	ret = pwd();
	if (!change_exist_var_in_dict("PWD", ret, g_term.env))
		die("Error while changing PWD variable.");
	free(ret);	
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
	update_pwd();
	i = 0;
	while (old->args[i])
		free(old->args[i++]);
	free(old->args);
	free(act);
	free(old);
}
