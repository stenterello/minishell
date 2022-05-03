/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gimartin <gimartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:33 by gimartin          #+#    #+#             */
/*   Updated: 2022/04/27 15:03:34 by gimartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*pwd(void)
{
	char	*path;
	int		i;

	i = 1;
	path = NULL;
	path = getcwd(path, i);
	while (path == NULL)
	{
		free(path);
		i++;
		path = getcwd(path, i);
	}
	g_term.last_exit = 0;
	return (path);
}
