/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 18:58:48 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/24 18:59:00 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	take_environ(void)
{
	int			i;
	t_dict		*new;
	t_dict		*prev;
	extern char	**environ;
	char		**tmp;

	i = 0;
	new = NULL;
	if (!g_term.env)
		malloc_and_check_dict(&g_term.env, 1);
	take_elem(g_term.env, &i);
	prev = g_term.env;
	tmp = environ;
	while (tmp[i])
	{
		malloc_and_check_dict(&new, 1);
		prev->next = new;
		prev = new;
		take_elem(new, &i);
	}
	change_exist_var_in_dict("SHELL", "minishell", g_term.env);
}
