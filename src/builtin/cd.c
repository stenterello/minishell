/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 21:55:20 by ddelladi          #+#    #+#             */
/*   Updated: 2022/06/15 16:30:36 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	go_to_old_pwd(int ret, t_command *old, char *act, t_terminfo *terminfo)
{
	ret = chdir(ft_getenv("OLDPWD", terminfo));
	if (ret == -1)
	{
		ft_putstr_fd(last_field(ft_getenv("SHELL", terminfo)), STDERR_FILENO);
		ft_putendl_fd(": cd: no OLDPWD in variables", STDERR_FILENO);
		terminfo->last_exit = 1;
		return ;
	}
	ft_putendl_fd(act, STDOUT_FILENO);
	save_old_pwd(old, act, terminfo);
	update_pwd(terminfo);
}

void	go_to_home(int ret, t_command *old, char *act, t_terminfo *terminfo)
{
	ret = chdir(ft_getenv("HOME", terminfo));
	if (ret == -1)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		terminfo->last_exit = 1;
		return ;
	}
	save_old_pwd(old, act, terminfo);
	update_pwd(terminfo);
}

int	get_home_path(char **dest, t_command *cmd, t_terminfo *terminfo)
{
	if (ft_getenv("HOME", terminfo) != NULL)
	{
		malloc_c(dest, ft_strlen(ft_getenv("HOME", terminfo)
				+ ft_strlen(cmd->args[1])));
		ft_strlcpy(*dest, ft_getenv("HOME", terminfo),
			ft_strlen(ft_getenv("HOME", terminfo)) + 1);
		ft_strlcat(*dest, &cmd->args[1][1], ft_strlen(*dest)
			+ ft_strlen(cmd->args[1]) + 1);
		*dest = get_path(*dest);
	}
	else
	{
		ft_putendl_fd("HOME not set", 2);
		return (1);
	}
	return (0);
}

void	go_to_dir(t_command *cmd, char *act,
	t_command *old, t_terminfo *terminfo)
{
	char		*dest;
	int			ret;

	dest = NULL;
	if (cmd->args[1][0] == '~')
	{
		if (get_home_path(&dest, cmd, terminfo))
			return ;
	}
	else
		dest = get_path(cmd->args[1]);
	ret = chdir(dest);
	if (ret == -1)
	{
		cd_error(dest, terminfo);
		return ;
	}
	save_old_pwd(old, act, terminfo);
	update_pwd(terminfo);
	free(dest);
}

void	cd(t_command *cmd, t_terminfo *terminfo)
{
	int			ret;
	char		*act;
	t_command	*old;

	ret = 0;
	act = pwd(terminfo);
	old = malloc(sizeof(t_command));
	if (!old)
		die("Malloc error");
	old->args = NULL;
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putendl_fd(": too many arguments", STDERR_FILENO);
		terminfo->last_exit = 1;
	}
	else if (cmd->args[1] && !ft_strncmp(cmd->args[1], "-\0", 2))
		go_to_old_pwd(ret, old, act, terminfo);
	else if (!cmd->args[1])
		go_to_home(ret, old, act, terminfo);
	else
		go_to_dir(cmd, act, old, terminfo);
}
