/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:17 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/15 15:49:19 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	save_old_pwd(t_command *old, char *act, t_terminfo *terminfo)
{
	int	i;

	terminfo->last_exit = 0;
	malloc_c_ptr(&old->args, 3);
	malloc_c(&old->args[0], 2);
	ft_strlcpy(old->args[0], "A", 2);
	malloc_c(&old->args[1], ft_strlen("OLDPWD=") + ft_strlen(act) + 1);
	ft_strlcpy(old->args[1], "OLDPWD=", ft_strlen("OLDPWD=") + 1);
	ft_strlcat(old->args[1], act,
		ft_strlen(old->args[1]) + ft_strlen(act) + 1);
	old->args[2] = NULL;
	export(old, terminfo);
	i = 0;
	while (old->args[i])
		free(old->args[i++]);
	free(old->args);
	free(act);
	free(old);
}

void	update_pwd(t_terminfo *terminfo)
{
	char	*ret;

	ret = pwd(terminfo);
	if (!change_exist_var_in_dict("PWD", ret, terminfo->env))
		die("Error while changing PWD variable.");
	free(ret);
}

void	cd_error(char *dest, t_terminfo *terminfo)
{
	ft_putstr_fd(ft_getenv("SHELL", terminfo), 2);
	ft_putstr_fd(": cd: ", 2);
	ft_putstr_fd(dest, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	terminfo->last_exit = 1;
}
