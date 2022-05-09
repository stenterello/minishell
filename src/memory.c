/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:08:51 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/09 12:16:43 by ddelladi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_dict(t_dict *env)
{
	t_dict	*tmp;
	t_dict	*tmp2;

	tmp = env;
	while (tmp)
	{
		tmp2 = tmp;
		if (tmp2->key)
			free(tmp2->key);
		else
			break ;
		if (tmp2->value)
			free(tmp2->value);
		tmp = tmp->next;
		free(tmp2);
	}
}

void	malloc_c(char **dst, int len)
{
	int	i;

	i = 0;
	*dst = ft_calloc(sizeof(char), len);
	if (!*dst)
		die("Malloc error");
	while ((*dst)[i])
		(*dst)[i++] = '\0';
}

void	malloc_c_ptr(char ***dst, int len)
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
	dst[0]->key = NULL;
	dst[0]->value = NULL;
	dst[0]->next = NULL;
}

void	malloc_and_check_list(t_list **dst, int len)
{
	*dst = malloc(sizeof(t_list) * len);
	if (!*dst)
		die("Malloc error");
}
