/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:37 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/15 15:49:02 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	delete_dict_var(t_dict *tmp, int *flag)
{
	free(tmp->key);
	free(tmp->value);
	free(tmp);
	*flag = 1;
}

void	search_list_and_delete(t_dict *tmp, t_dict *tmp2,
	char *search, int flag)
{
	while (tmp && !flag && tmp->next)
	{
		tmp2 = tmp->next;
		if (!ft_strncmp(tmp2->key, search, ft_strlen(search) + 1))
		{
			tmp->next = tmp2->next;
			delete_dict_var(tmp2, &flag);
			break ;
		}
		tmp = tmp->next;
	}
}

void	search_and_delete(char *search, t_terminfo *terminfo)
{
	t_dict	*tmp;
	t_dict	*tmp2;
	int		flag;

	flag = 0;
	if (!terminfo->env)
		return ;
	tmp = terminfo->env;
	tmp2 = tmp;
	if (!ft_strncmp(tmp->key, search, ft_strlen(search) + 1))
	{
		terminfo->env = tmp->next;
		delete_dict_var(tmp, &flag);
	}
	search_list_and_delete(tmp, tmp2, search, flag);
	if (!terminfo->var || !terminfo->var->key || flag)
		return ;
	tmp = terminfo->var;
	if (!ft_strncmp(tmp->key, search, ft_strlen(search) + 1))
	{
		terminfo->var = tmp->next;
		delete_dict_var(tmp, &flag);
	}
	search_list_and_delete(tmp, tmp2, search, flag);
}

void	unset(t_command *cmd, t_terminfo *terminfo)
{
	int		i;
	int		ind;
	char	*search;

	search = NULL;
	ind = 1;
	while (cmd->args[ind])
	{
		i = ft_strlen(cmd->args[ind]);
		if (i != 0)
		{
			malloc_c(&search, i + 1);
			ft_strlcpy(search, cmd->args[ind], i + 1);
			search_and_delete(search, terminfo);
			free(search);
		}
		ind++;
	}
	terminfo->last_exit = 0;
}
