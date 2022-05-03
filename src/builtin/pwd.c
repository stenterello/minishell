/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddelladi <ddelladi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 15:03:33 by gimartin          #+#    #+#             */
/*   Updated: 2022/05/03 21:56:00 by ddelladi         ###   ########.fr       */
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
