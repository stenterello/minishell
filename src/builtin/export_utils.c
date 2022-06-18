/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:36:08 by gimartin          #+#    #+#             */
/*   Updated: 2022/06/15 15:48:51 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_export_syntax(t_command *cmd, int ind, t_terminfo *terminfo)
{
	if (!ft_isalnum(cmd->args[ind][0]))
	{
		ft_putstr_fd(ft_getenv("SHELL", terminfo), 2);
		ft_putstr_fd(": export: \"", 2);
		ft_putstr_fd(cmd->args[ind], 2);
		ft_putendl_fd("\": not a valid identifier", 2);
		terminfo->last_exit = 1;
		return (1);
	}
	return (0);
}

void	search_value_for_key(t_command *cmd, int *i,
	int ind, t_terminfo *terminfo)
{
	char	*ret;

	if (*i == -1)
	{
		ret = search_vars_value(cmd->args[ind], terminfo->var);
		if (ret != NULL)
		{
			*i = value_len(ret);
			free(ret);
		}
	}
}

void	insert_empty_var(t_dict *new, t_terminfo *terminfo)
{
	t_dict	*tmp;

	tmp = terminfo->env;
	while (tmp)
	{
		if (!ft_strncmp(new->key, terminfo->env->key, ft_strlen(new->key) + 1))
			return ;
		if (!tmp->next)
		{
			tmp->next = new;
			tmp = tmp->next;
			tmp->value = NULL;
			tmp->next = NULL;
			return ;
		}
		tmp = tmp->next;
	}
}

void	save_concatenation(char *dst, char *src1, char *src2, t_dict *save_dict)
{
	malloc_c(&dst, ft_strlen(src1) + ft_strlen(src2) + 1);
	ft_strlcpy(dst, src1, ft_strlen(src1) + 1);
	ft_strlcat(dst, src2, ft_strlen(dst) + ft_strlen(src2) + 1);
	free(save_dict->value);
	save_dict->value = dst;
}

void	export_to_env_vars(t_dict *new, t_terminfo *terminfo)
{
	if (!change_exist_var_in_dict(new->key, new->value, terminfo->env))
		insert_into_vars(new->key, new->value, terminfo->env);
	free(new->key);
	free(new->value);
	free(new);
	terminfo->last_exit = 0;
}
