/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:37 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/13 10:13:31 by ddelladi         ###   ########.fr       */
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

void	sup_sd(t_dict *tmp, t_dict *tmp2, char *search, int flag)
{
	while (tmp && !flag && tmp->next)
	{
		tmp2 = tmp->next;
		if (!ft_strncmp(tmp2->key, search, ft_strlen(search)))
		{
			tmp->next = tmp2->next;
			delete_dict_var(tmp2, &flag);
			break ;
		}
		tmp = tmp->next;
	}
}

void	sup_sd2(t_dict *tmp, t_dict *tmp2, int flag, char *search)
{
	tmp = g_term.var;
	if (!ft_strncmp(tmp->key, search, ft_strlen(search)))
	{
		g_term.var = tmp->next;
		delete_dict_var(tmp, &flag);
	}
	while (tmp->next && !flag)
	{
		tmp2 = tmp->next;
		if (!ft_strncmp(tmp2->key, search, ft_strlen(search)))
		{
			tmp->next = tmp2->next;
			delete_dict_var(tmp2, &flag);
			break ;
		}
		tmp = tmp->next;
	}
}

void	search_and_delete(char *search)
{
	t_dict	*tmp;
	t_dict	*tmp2;
	int		flag;

	flag = 0;
	if (!g_term.env)
		return ;
	tmp = g_term.env;
	tmp2 = tmp;
	if (!ft_strncmp(tmp->key, search, ft_strlen(search)))
	{
		g_term.env = tmp->next;
		delete_dict_var(tmp, &flag);
	}
	sup_sd(tmp, tmp2, search, flag);
	if (!flag && g_term.var && g_term.var->key)
		sup_sd2(tmp, tmp2, flag, search);
}

void	unset(t_command *cmd)
{
	int		i;
	char	*search;

	search = NULL;
	if (!cmd->args[1])
		return ;
	i = ft_strlen(cmd->args[1]);
	if (i == 0)
		return ;
	malloc_c(&search, i + 1);
	ft_strlcpy(search, cmd->args[1], i + 1);
	search_and_delete(search);
	free(search);
	g_term.last_exit = 0;
}
